/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include <future>

#include "execution_manager.h"

#include "executor_delete.h"
#include "executor_index_scan.h"
#include "executor_insert.h"
#include "executor_nestedloop_join.h"
#include "executor_projection.h"
#include "executor_seq_scan.h"
#include "executor_update.h"
#include "index/ix.h"
#include "record_printer.h"

std::vector<std::future<void>> futures;

const char* help_info =
    "Supported SQL syntax:\n"
    "  command ;\n"
    "command:\n"
    "  CREATE TABLE table_name (column_name type [, column_name type ...])\n"
    "  DROP TABLE table_name\n"
    "  CREATE INDEX table_name (column_name)\n"
    "  DROP INDEX table_name (column_name)\n"
    "  SHOW INDEX table_name\n"
    "  INSERT INTO table_name VALUES (value [, value ...])\n"
    "  DELETE FROM table_name [WHERE where_clause]\n"
    "  UPDATE table_name SET column_name = value [, column_name = value ...] "
    "[WHERE where_clause]\n"
    "  SELECT selector FROM table_name [WHERE where_clause]\n"
    "type:\n"
    "  {INT | FLOAT | CHAR(n)}\n"
    "where_clause:\n"
    "  condition [AND condition ...]\n"
    "condition:\n"
    "  column op {column | value}\n"
    "column:\n"
    "  [table_name.]column_name\n"
    "op:\n"
    "  {= | <> | < | > | <= | >=}\n"
    "selector:\n"
    "  {* | column [, column ...]}\n";

// 主要负责执行DDL语句
void QlManager::run_mutli_query(std::shared_ptr<Plan> plan, Context* context) {
    if (auto x = std::dynamic_pointer_cast<DDLPlan>(plan)) {
        switch (x->tag) {
            case T_CreateTable: {
                sm_manager_->create_table(x->tab_name_, x->cols_, context);
                break;
            }
            case T_DropTable: {
                sm_manager_->drop_table(x->tab_name_, context);
                break;
            }
            case T_CreateIndex: {
                sm_manager_->create_index(x->tab_name_, x->tab_col_names_,
                                          context);
                break;
            }
            case T_DropIndex: {
                sm_manager_->drop_index(x->tab_name_, x->tab_col_names_,
                                        context);
                break;
            }
            case T_ShowIndex: {
                sm_manager_->show_index(x->tab_name_, context);
                break;
            }
            case T_LoadData: {
                auto future_result = std::async(
                    std::launch::async,
                    [this, file_apth = x->file_path_, tab_name = x->tab_name_,
                     context]() {  // 拷贝捕获
                        sm_manager_->load_record(file_apth, tab_name, context);
                    });
                futures.push_back(std::move(future_result));
                // sm_manager_->load_record(x->file_path_, x->tab_name_, context);
                break;
            }
            default:
                throw InternalError("Unexpected field type");
                break;
        }
    }
}

// 执行help; show tables; desc table; begin; commit; abort;语句
void QlManager::run_cmd_utility(std::shared_ptr<Plan> plan, txn_id_t* txn_id,
                                Context* context) {
    if (auto x = std::dynamic_pointer_cast<OtherPlan>(plan)) {
        switch (x->tag) {
            case T_Help: {
                memcpy(context->data_send_ + *(context->offset_), help_info,
                       strlen(help_info));
                *(context->offset_) = strlen(help_info);
                break;
            }
            case T_ShowTable: {
                sm_manager_->show_tables(context);
                break;
            }
            case T_DescTable: {
                sm_manager_->desc_table(x->tab_name_, context);
                break;
            }
            case T_Transaction_begin: {
                // 显示开启一个事务
                context->txn_->set_txn_mode(true);
                break;
            }
            case T_Transaction_commit: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->commit(context->txn_, context->log_mgr_);
                break;
            }
            case T_Transaction_rollback: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->abort(context, context->log_mgr_);
                break;
            }
            case T_Transaction_abort: {
                context->txn_ = txn_mgr_->get_transaction(*txn_id);
                txn_mgr_->abort(context, context->log_mgr_);
                break;
            }
            default:
                throw InternalError("Unexpected field type");
                break;
        }

    } else if (auto x = std::dynamic_pointer_cast<SetKnobPlan>(plan)) {
        switch (x->set_knob_type_) {
            case ast::SetKnobType::EnableNestLoop: {
                planner_->set_enable_nestedloop_join(x->bool_value_);
                break;
            }
            case ast::SetKnobType::EnableSortMerge: {
                planner_->set_enable_sortmerge_join(x->bool_value_);
                break;
            }
            default: {
                throw RMDBError("Not implemented!\n");
                break;
            }
        }
    }
}

// 执行select语句，select语句的输出除了需要返回客户端外，还需要写入output.txt文件中
void QlManager::select_from(std::unique_ptr<AbstractExecutor> executorTreeRoot,
                            std::vector<TabCol> sel_cols, Context* context) {
    // 先判断futures中的load是否全部结束
    for (auto& future : futures) {
        future.get();
    }
    futures.clear();
    std::vector<std::string> captions;
    captions.reserve(sel_cols.size());
    for (auto& sel_col : sel_cols) {
        if (!sel_col.as_name.empty())
            captions.push_back(sel_col.as_name);
        else
            captions.push_back(sel_col.col_name);
    }

    // Print header into buffer
    RecordPrinter rec_printer(sel_cols.size());
    rec_printer.print_separator(context);
    rec_printer.print_record(captions, context);
    rec_printer.print_separator(context);

    // 如果output_ellipsis_为true，则不输出到文件，否则输出到文件
    std::fstream outfile;
    if (!context->output_ellipsis_) {
        outfile.open("output.txt", std::ios::out | std::ios::app);
        outfile << "|";
        for (const auto& caption : captions) {
            outfile << " " << caption << " |";
        }
        outfile << "\n";
    }

    // Print records
    size_t num_rec = 0;

    // 执行query_plan

    auto formatFloat = [](float f) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << f;
        return oss.str();
    };

    for (executorTreeRoot->beginTuple(); !executorTreeRoot->is_end();
         executorTreeRoot->nextTuple()) {
        auto Tuple = executorTreeRoot->Next();
        std::vector<std::string> columns;
        for (auto& col : executorTreeRoot->cols()) {
            std::string col_str;
            if (col.type == TYPE_INT) {
                col_str =
                    std::to_string(Tuple->import <std::int64_t>(col.offset));
            } else if (col.type == TYPE_FLOAT) {
                col_str = std::to_string(Tuple->import <double>(col.offset));
            } else if (col.type == TYPE_STRING) {
                col_str = Tuple->import <std::string>(col.offset, col.len);
                col_str.resize(strlen(col_str.c_str()));
            }
            columns.push_back(col_str);
        }
        // print record into buffer
        rec_printer.print_record(columns, context);
        // print record into file
        if (!context->output_ellipsis_) {
            outfile << "|";
            for (const auto& col : columns) {
                outfile << " " << col << " |";
            }
            outfile << "\n";
        }
        num_rec++;
    }
    if (!context->output_ellipsis_) {
        outfile.close();
    }
    // Print footer into buffer
    rec_printer.print_separator(context);
    // Print record count into buffer
    RecordPrinter::print_record_count(num_rec, context);
}

SelectResult QlManager::select_from_and_get_return(
    std::unique_ptr<AbstractExecutor> executorTreeRoot,
    std::vector<TabCol> sel_cols, Context* context) {
    std::vector<ColMeta> cols = executorTreeRoot->cols();
    std::vector<RmRecord> records;
    // 执行query_plan
    for (executorTreeRoot->beginTuple(); !executorTreeRoot->is_end();
         executorTreeRoot->nextTuple()) {
        auto Tuple = executorTreeRoot->Next();
        RmRecord record(*Tuple);
        records.push_back(std::move(record));
    }
    return std::make_pair(std::move(cols), std::move(records));
}

// 执行DML语句
void QlManager::run_dml(std::unique_ptr<AbstractExecutor> exec) {
    exec->Next();
}