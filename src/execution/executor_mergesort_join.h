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

        left_records.clear();
        right_records.clear();
        joined_records.clear();

        current_tuple = nullptr;
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

        for (int i = 0, p = 0;
             i < (int)left_records.size() && p < (int)right_records.size();
             i++) {
            std::unique_ptr<RmRecord> now_joined_record =
                get_joined_record(left_records[i], right_records[p]);
            while (comp_cond(cols_, fed_cond_, now_joined_record.get()) == 1) {
                p++;
                if (p >= (int)right_records.size())
                    break;
                now_joined_record =
                    get_joined_record(left_records[i], right_records[p]);
            }
            if (p >= (int)right_records.size())
                break;
            int rem = p;
            if (comp_cond(cols_, fed_cond_, now_joined_record.get()) == 0) {
                while (comp_cond(cols_, fed_cond_, now_joined_record.get()) ==
                       0) {
                    joined_records.emplace_back(std::move(now_joined_record));
                    p++;
                    if (p >= (int)right_records.size())
                        break;
                    now_joined_record =
                        get_joined_record(left_records[i], right_records[p]);
                }
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

    ExecutorType getType() const override {
        return ExecutorType::NESTED_LOOP_JOIN;
    }
};