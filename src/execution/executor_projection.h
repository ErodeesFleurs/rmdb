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

class ProjectionExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;        // 投影节点的儿子节点
    std::vector<ColMeta> cols_;                     // 需要投影的字段
    size_t len_;                                    // 字段总长度
    std::vector<size_t> sel_idxs_;
    bool prev_is_aggr_ = false;

   public:
    ProjectionExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol> &sel_cols) {
        prev_ = std::move(prev);

        if (prev_->getType() == "AggregateExecutor") {
            prev_is_aggr_ = true;
        }

        size_t curr_offset = 0;
        auto &prev_cols = prev_->cols();
        for (auto& col : prev_cols) {
            std::cerr << "ProjectionExecutor: " << col.name << " " << col.tab_name << " " << col.offset << " " << col.type << std::endl;
        }
        for (auto &sel_col : sel_cols) {
            auto pos = get_col(prev_cols, sel_col);
            sel_idxs_.push_back(pos - prev_cols.begin());
            auto col = *pos;
            col.offset = curr_offset;
            curr_offset += col.len;
            cols_.push_back(col);
        }
        if (prev_is_aggr_) {
            cols_ = prev_cols;
        }
        len_ = curr_offset;
    }

    const std::vector<ColMeta> &cols() const override {
        return cols_;
    }

    void beginTuple() override {
        std::cerr << "Projection BeginTuple" << std::endl;
        prev_->beginTuple(); // 调用子节点的beginTuple
    }

    void nextTuple() override {
        prev_->nextTuple();
    }

    std::unique_ptr<RmRecord> Next() override {
        std::cerr << "Projection Next" << std::endl;
        auto rec = std::make_unique<RmRecord>(len_);
        auto& prev_cols = prev_->cols();
        auto prev_rec = prev_->Next();
        for (size_t i = 0; i < sel_idxs_.size(); i++) { // 从prev_rec中取出需要的字段
            auto idx = sel_idxs_[i];
            if (prev_is_aggr_) {
                idx = i;
            }
            auto col = cols_[i];
            auto prev_col = prev_cols[idx];
            auto prev_val = prev_rec->data + prev_col.offset;
            std::cerr << "Projection Next: " << idx << " " <<  prev_col.name << " " << prev_col.tab_name << " " << prev_col.offset << " " << prev_col.type << " " << prev_col.len << std::endl;
            auto val = rec->data + col.offset;
            memcpy(val, prev_val, col.len);
        }
        return rec;
    }

    bool is_end() const override {
        return prev_->is_end();
    }

    Rid &rid() override { return _abstract_rid; }
};