/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

extern int count_index_scan;
extern std::map<std::string, std::atomic_bool> is_thread_ok;
class IndexScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;              // 表名称
    TabMeta tab_;                       // 表的元数据
    std::vector<Condition> conds_;      // 扫描条件
    RmFileHandle* fh_;                  // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // 需要读取的字段
    size_t len_;                        // 选取出来的一条记录的长度
    std::vector<Condition> fed_conds_;  // 扫描条件，和conds_字段相同

    std::vector<std::string>
        index_col_names_;   // index scan涉及到的索引包含的字段
    IndexMeta index_meta_;  // index scan涉及到的索引元数据

    Rid rid_;
    std::unique_ptr<RecScan> scan_;
    IxIndexHandle* ih;
    IxManager* im;
    int index_count;  // index scan涉及到的索引数量

    SmManager* sm_manager_;

   public:
    IndexScanExecutor(SmManager* sm_manager, std::string tab_name,
                      std::vector<Condition> conds,
                      std::vector<std::string> index_col_names,
                      Context* context) {
        sm_manager_ = sm_manager;
        context_ = context;
        tab_name_ = std::move(tab_name);
        tab_ = sm_manager_->db_.get_table(tab_name_);
        conds_ = std::move(conds);
        // index_no_ = index_no;

        im = sm_manager->get_ix_manager();
        std::string ix_name = im->get_index_name(tab_name_, index_col_names);
        if (!sm_manager->ihs_.count(ix_name)) {
            //如果没有打开则打开文件
            sm_manager->ihs_.emplace(
                ix_name, im->open_index(tab_name_, index_col_names));
        }
        ih = sm_manager->ihs_[ix_name].get();

        index_col_names_ = index_col_names;
        index_meta_ = *(tab_.get_index_meta(index_col_names_));
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab_.cols;
        len_ = cols_.back().offset + cols_.back().len;
        std::map<CompOp, CompOp> swap_op = {{OP_EQ, OP_EQ}, {OP_NE, OP_NE},
                                            {OP_LT, OP_GT}, {OP_GT, OP_LT},
                                            {OP_LE, OP_GE}, {OP_GE, OP_LE}};

        for (auto& cond : conds_) {
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
        }
        fed_conds_ = conds_;
        // if (context_->txn_ != nullptr) {
        //     context_->lock_mgr_->lock_IS_on_table(
        //         context_->txn_, sm_manager_->fhs_[tab_name_]->GetFd());
        // }
    }

    void beginTuple() override {
        std::string ix_name = sm_manager_->get_ix_manager()->get_index_name(
            tab_name_, index_col_names_);
        RmRecord lower_record(index_meta_.col_tot_len),
            upper_record(index_meta_.col_tot_len);
        int offset = 0;
        for (auto col : index_meta_.cols) {
            Value max_value, min_value;
            if (col.type == TYPE_INT) {
                max_value.set_int(INT32_MAX);
                min_value.set_int(INT32_MIN);
            } else if (col.type == TYPE_FLOAT) {
                max_value.set_float(__DBL_MAX__);
                min_value.set_float(__DBL_MIN__);
            } else if (col.type == TYPE_STRING) {
                max_value.set_str(std::string(col.len, 255));
                min_value.set_str(std::string(col.len, 0));
            }
            for (const auto& cond : fed_conds_) {
                if (cond.lhs_col.col_name != col.name || !cond.is_rhs_val)
                    continue;
                if (cond.op == OP_EQ) {
                    if (check_cond(cond.rhs_val, min_value, OP_GT)) {
                        min_value = cond.rhs_val;
                    }
                    if (check_cond(cond.rhs_val, max_value, OP_LT)) {
                        max_value = cond.rhs_val;
                    }
                } else if (cond.op == OP_LT &&
                           check_cond(cond.rhs_val, max_value, OP_LT)) {
                    max_value = cond.rhs_val;
                } else if (cond.op == OP_LE &&
                           check_cond(cond.rhs_val, max_value, OP_LT)) {
                    max_value = cond.rhs_val;
                } else if (cond.op == OP_GT &&
                           check_cond(cond.rhs_val, min_value, OP_GT)) {
                    min_value = cond.rhs_val;
                } else if (cond.op == OP_GE &&
                           check_cond(cond.rhs_val, min_value, OP_GT)) {
                    min_value = cond.rhs_val;
                } else if (cond.op == OP_NE) {
                    // do nothing
                }
            }
            min_value.raw = nullptr;
            max_value.raw = nullptr;
            syncType(min_value, col.type, false);
            syncType(max_value, col.type, true);
            if (min_value.raw == nullptr) {
                if (min_value.type == TYPE_STRING) {
                    min_value.init_raw(col.len);
                } else if (min_value.type == TYPE_INT) {
                    min_value.init_raw(sizeof(int));
                } else if (min_value.type == TYPE_FLOAT) {
                    min_value.init_raw(sizeof(float));
                }
            }
            if (max_value.raw == nullptr) {
                if (max_value.type == TYPE_STRING) {
                    max_value.init_raw(col.len);
                } else if (max_value.type == TYPE_INT) {
                    max_value.init_raw(sizeof(int));
                } else if (max_value.type == TYPE_FLOAT) {
                    max_value.init_raw(sizeof(float));
                }
            }
            memcpy(upper_record.data + offset, max_value.raw->data, col.len);
            memcpy(lower_record.data + offset, min_value.raw->data, col.len);
            offset += col.len;
        }
        auto start = ih->lower_bound(lower_record.data);
        auto end = ih->upper_bound(upper_record.data);
        scan_ =
            std::make_unique<IxScan>(ih, start, end, sm_manager_->get_bpm());
        // scan_ = std::make_unique<RmScan>(fh_);  // it's maigc
        while (!is_end()) {
            count_index_scan++;
            rid_ = scan_->rid();
            auto rec = fh_->get_record(rid_, context_);
            if (fed_conds_.empty() ||
                eval_conds(cols_, fed_conds_, rec.get())) {
                break;
            }
            scan_->next();
        }
    }

    void nextTuple() override {
        if (!is_end()) {
            scan_->next();
        }
        while (!is_end()) {
            count_index_scan++;
            rid_ = scan_->rid();
            try {
                auto record = fh_->get_record(rid_, context_);
                if (fed_conds_.empty() ||
                    eval_conds(cols_, fed_conds_, record.get())) {
                    break;
                }
            } catch (RecordNotFoundError& e) {
                std::cerr << e.what() << std::endl;
            }
            scan_->next();
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        return fh_->get_record(rid_, context_);
    }

    bool is_end() const override {
        if (scan_->is_end())
            return true;
        return false;
    }

    const std::vector<ColMeta>& cols() const override { return cols_; }

    Rid& rid() override { return rid_; }

    ExecutorType getType() const override { return ExecutorType::INDEX_SCAN; }

    static void syncType(Value& lhs, ColType need_type, bool is_cell = false) {
        if (lhs.type == TYPE_FLOAT && need_type == TYPE_INT) {
            if (is_cell) {
                lhs.to_cell();
            } else {
                lhs.to_floor();
            }
            lhs.to_int();
        } else if (lhs.type == TYPE_INT && need_type == TYPE_FLOAT) {
            lhs.to_float();
        }
    }
};