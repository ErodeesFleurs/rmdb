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

class NestedLoopJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;  // 左儿子节点（需要join的表）
    std::unique_ptr<AbstractExecutor> right_;  // 右儿子节点（需要join的表）
    size_t len_;                 // join后获得的每条记录的长度
    std::vector<ColMeta> cols_;  // join后获得的记录的字段

    std::vector<Condition> fed_conds_;  // join条件
    bool is_end_;

   public:
    NestedLoopJoinExecutor(std::unique_ptr<AbstractExecutor> left,
                           std::unique_ptr<AbstractExecutor> right,
                           std::vector<Condition> conds) {
        std::cerr << "NestedLoopJoinExecutor" << std::endl;
        left_ = std::move(left);
        right_ = std::move(right);
        len_ = left_->tupleLen() + right_->tupleLen();
        cols_ = left_->cols();
        auto right_cols = right_->cols();
        for (auto& col : right_cols) {
            col.offset += left_->tupleLen();
        }

        cols_.insert(cols_.end(), right_cols.begin(), right_cols.end());
        is_end_ = false;
        fed_conds_ = std::move(conds);
    }

    const std::vector<ColMeta>& cols() const override { return cols_; }

    void beginTuple() override {
        std::cerr << "NestedLoopJoin BeginTuple" << std::endl;
        left_->beginTuple();
        right_->beginTuple();
        if (left_->is_end() || right_->is_end()) {
            is_end_ = true;
            return;
        }
        find_record();
    }

    void nextTuple() override {
        if (is_end())
            return;
        left_->nextTuple();
        if (left_->is_end()) {
            right_->nextTuple();
            left_->beginTuple();
        }
        find_record();
    }

    std::unique_ptr<RmRecord> Next() override {
        auto record = std::make_unique<RmRecord>(len_);
        auto left_record = left_->Next();
        auto right_record = right_->Next();
        memcpy(record->data, left_record->data, left_->tupleLen());
        memcpy(record->data + left_->tupleLen(), right_record->data,
               right_->tupleLen());
        return record;
    }

    bool is_end() const override { return is_end_; }

    Rid& rid() override { return _abstract_rid; }

    void find_record() {
        while (!right_->is_end()) {
            auto record = std::make_unique<RmRecord>(len_);
            auto left_record = left_->Next();
            auto right_record = right_->Next();
            memcpy(record->data, left_record->data, left_->tupleLen());
            memcpy(record->data + left_->tupleLen(), right_record->data,
                   right_->tupleLen());
            if (fed_conds_.empty() ||
                eval_conds(cols_, fed_conds_, record.get())) {
                return;
            }
            left_->nextTuple();
            if (left_->is_end()) {
                right_->nextTuple();
                left_->beginTuple();
            }
        }
        is_end_ = true;
    }

    ExecutorType getType() const override {
        return ExecutorType::NESTED_LOOP_JOIN;
    }
};