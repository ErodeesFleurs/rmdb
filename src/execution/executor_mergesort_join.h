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
#include "system/sm_manager.h"
#include <string>

class MergeSortJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;  // 左儿子节点（需要join的表）
    std::unique_ptr<AbstractExecutor> right_;  // 右儿子节点（需要join的表）
    size_t len_;                 // join后获得的每条记录的长度
    std::vector<ColMeta> cols_;  // join后获得的记录的字段

    std::vector<Condition> fed_conds_;  // join条件
    Condition fed_cond_;

    std::vector<std::unique_ptr<RmRecord>> left_records;
    std::vector<std::unique_ptr<RmRecord>> right_records;

    std::vector<std::unique_ptr<RmRecord>> joined_records;
    std::vector<std::unique_ptr<RmRecord>>::iterator joined_records_iterator;
    std::unique_ptr<RmRecord> current_tuple;

    std::vector<ColMeta>::const_iterator left_col;
    std::vector<ColMeta>::const_iterator right_col;
    int left_offset;
    int right_offset;
    ColType left_type;
    ColType right_type;

   public:
    MergeSortJoinExecutor(std::unique_ptr<AbstractExecutor> left,
                          std::unique_ptr<AbstractExecutor> right,
                          std::vector<Condition> conds) {
        std::cerr << "MergeSortJoinExecutor" << std::endl;
        left_ = std::move(left);
        right_ = std::move(right);
        len_ = left_->tupleLen() + right_->tupleLen();
        cols_ = left_->cols();
        auto right_cols = right_->cols();
        for (auto& col : right_cols) {
            col.offset += left_->tupleLen();
        }

        cols_.insert(cols_.end(), right_cols.begin(), right_cols.end());
        fed_conds_ = std::move(conds);
        assert(fed_conds_.size() == 1);
        fed_cond_ = fed_conds_.front();
        
        assert(!fed_cond_.is_rhs_list && !fed_cond_.is_rhs_query && !fed_cond_.is_rhs_val);

        left_records.clear();
        right_records.clear();
        joined_records.clear();

        current_tuple = nullptr;

        left_col = get_col(left_->cols(), fed_cond_.lhs_col);
        left_offset = left_col->offset;
        left_type = left_col->type;
        right_col = get_col(right_->cols(), fed_cond_.rhs_col);
        right_offset = right_col->offset;
        right_type = right_col->type;
        assert(fed_cond_.op == OP_EQ);
    }

    const std::vector<ColMeta>& cols() const override { return cols_; }

    void beginTuple() override {
        std::cerr << "MergeSortJoin BeginTuple" << std::endl;
        left_->beginTuple();
        right_->beginTuple();
        while (!left_->is_end()) {
            left_records.emplace_back(left_->Next());
            left_->nextTuple();
        }
        while (!right_->is_end()) {
            right_records.emplace_back(right_->Next());
            right_->nextTuple();
        }

        // std::cerr << "begin print_table_into_one_file_respectively" << std::endl;
        print_table_into_one_file_respectively();
        // std::cerr << "end print_table_into_one_file_respectively" << std::endl;

        for (int i = 0, p = 0; i < (int)left_records.size() && p < (int)right_records.size(); i++) {
            while (p < (int)right_records.size() && comp_cond(left_records[i].get(), right_records[p].get()) == 1) {
                p++;
            }
            int rem = p;
            while (p < (int)right_records.size() && comp_cond(left_records[i].get(), right_records[p].get()) ==
                    0) {
                std::unique_ptr<RmRecord> now_joined_record = std::move(get_joined_record(left_records[i], right_records[p]));
                joined_records.emplace_back(std::move(now_joined_record));
                p++;
            }
            p = rem;
        }

        // std::cerr << joined_records.size() << "!!!" << std::endl;
        joined_records_iterator = joined_records.begin();

        if (!is_end()) {
            // std::cerr << "[][][] -> " << joined_records.begin()->get()->size << ' ' << joined_records_iterator->get()->data << std::endl;
            current_tuple =
                std::make_unique<RmRecord>(*((*joined_records_iterator).get()));
        }
    }

    int comp_cond(const RmRecord* left_rec, const RmRecord* right_rec) {
        char* lhs = left_rec->data + left_offset;
        char* rhs = right_rec->data + right_offset;
        int cmp;
        if (left_type != right_type) {
            Value ls = get_value(left_type, lhs);
            Value rs = get_value(right_type, rhs);
            cmp = val_compare(ls, rs);
        } else {
            cmp = ix_compare(lhs, rhs, right_type, left_col->len);
        }
        return cmp;
    }

    void nextTuple() override {
        if (!is_end()) {
            joined_records_iterator++;
            if (!is_end()) {
                current_tuple = std::make_unique<RmRecord>(
                    *((*joined_records_iterator).get()));
            }
        }
    }

    std::unique_ptr<RmRecord> get_joined_record(
        std::unique_ptr<RmRecord>& left_record,
        std::unique_ptr<RmRecord>& right_record) {
        auto record = std::make_unique<RmRecord>(len_);
        memcpy(record->data, left_record->data, left_->tupleLen());
        memcpy(record->data + left_->tupleLen(), right_record->data,
               right_->tupleLen());
        return record;
    }

    std::unique_ptr<RmRecord> Next() override {
        return std::move(current_tuple);
    }

    bool is_end() const override {
        return joined_records_iterator == joined_records.end();
    }

    Rid& rid() override { return _abstract_rid; }

    size_t tupleLen() const override { return len_; }

    void print_table_into_one_file_respectively() {
        std::fstream outfile;
        outfile.open("sorted_results.txt", std::ios::out | std::ios::app);

        auto print_one_table = [&](std::unique_ptr<AbstractExecutor>& prev_, std::vector<std::unique_ptr<RmRecord>>& records) {
            const auto& cols = prev_->cols();
            std::vector<std::string> captions;
            captions.reserve(cols.size());
            for (auto& col : cols) {
                captions.push_back(col.name);
            }
            outfile << "|";
            for (int i = 0; i < (int)captions.size(); ++i) {
                outfile << " " << captions[i] << " |";
            }
            outfile << "\n";

            for (int i = 0; i < (int)records.size(); i++) {
                
                std::vector<std::string> columns;
                for (auto& col : prev_->cols()) {
                    std::string col_str;
                    char* rec_buf = records[i]->data + col.offset;
                    if (col.type == TYPE_INT) {
                        col_str = std::to_string(*(int*)rec_buf);
                    } else if (col.type == TYPE_FLOAT) {
                        col_str = std::to_string(*(double*)rec_buf);
                    } else if (col.type == TYPE_STRING) {
                        col_str = std::string((char*)rec_buf, col.len);
                        col_str.resize(strlen(col_str.c_str()));
                    }
                    columns.push_back(col_str);
                }
                outfile << "|";
                for (int i = 0; i < (int)columns.size(); ++i) {
                    outfile << " " << columns[i] << " |";
                }
                outfile << "\n";
            }
        };

        print_one_table(left_, left_records);
        print_one_table(right_, right_records);
        
        outfile.close();
    }

    ExecutorType getType() const override {
        return ExecutorType::NESTED_LOOP_JOIN;
    }
};