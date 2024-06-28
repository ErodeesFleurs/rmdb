#pragma once
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

#include <unordered_map>

class GroupExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;
    std::vector<ColMeta> cols_;                    
    std::vector<TabCol> group_cols_;
    std::vector<Condition> having_conds_;
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> grouped_records;
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator>::iterator current_group;
    std::unique_ptr<RmRecord> current_tuple;

   public:
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator> group_iterators;

    GroupExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol>& sel_cols, const std::vector<TabCol>& group_cols, std::vector<Condition> conds) {
        prev_ = std::move(prev);
        for (const auto& sel_col : sel_cols) {
            cols_.push_back(*prev_->get_col(prev_->cols(), sel_col));
        }
        having_conds_ = conds;
        group_cols_ = group_cols;
        current_tuple = nullptr;
    }

    void beginTuple() override {
        std::cerr << "Group BeginTuple" << std::endl;
        prev_->beginTuple();
        grouped_records.clear();
        group_iterators.clear();

        while (!prev_->is_end()) {
            auto tuple = prev_->Next();
            std::string group_key = generateGroupKey(tuple);
            grouped_records[group_key].emplace_back(std::move(tuple));
            prev_->nextTuple();
        }
        // 条件过滤
        std::vector<std::string> group_keys_to_remove;
        for (auto& [key, records] : grouped_records) {
            if (having_conds_.empty()) {
                break;
            }
            bool should_remove = false;
            should_remove = !eval_aggr_conds(cols_, having_conds_, records);
            if (should_remove) {
                group_keys_to_remove.push_back(key);
            }
        }
        // 删除不满足条件的组
        for (const auto& key : group_keys_to_remove) {
            grouped_records.erase(key);
        }
        
        for (auto it = grouped_records.begin(); it != grouped_records.end(); ++it) {
            group_iterators.push_back(it);
        }
        std::reverse(group_iterators.begin(), group_iterators.end());

        current_group = group_iterators.begin();
        // 初始化current_tuple
        if (current_group != group_iterators.end()) {
            auto& front = current_group->operator->()->second.front();
            auto temp_tuple = std::make_unique<RmRecord>(front->size, front->data);
            current_tuple = std::move(temp_tuple);
        }
    }

    void nextTuple() override {
        if (current_group != group_iterators.end()) {
            current_group++;
            if (current_group != group_iterators.end()) {
                auto& front = current_group->operator->()->second.front();
                auto temp_tuple = std::make_unique<RmRecord>(front->size, front->data);
                current_tuple = std::move(temp_tuple);
            }
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        return std::move(current_tuple);
    }

    const std::vector<ColMeta>& cols() const override {
        return prev_->cols();
    }

    Rid& rid() override {
        return _abstract_rid;
    }

    bool is_end() const override {
        return current_group == group_iterators.end();
    }

   private:

   std::string generateGroupKey(std::unique_ptr<RmRecord>& record) {
        std::string group_key = "";
        for (const auto& group_col : group_cols_) {
            const auto col_meta = prev_->get_col(prev_->cols(), group_col);
            const char* col_data = record->data + col_meta->offset;
            group_key += std::string(col_data, col_meta->len);
        }
        // std::cerr << "Group key: " << group_key << std::endl;
        return group_key;
    }

    bool eval_aggr_cond(const std::vector<ColMeta>& rec_cols, const Condition& cond, std::vector<std::unique_ptr<RmRecord>>& rec) {
        auto copy_cond = cond;
        if (copy_cond.lhs_col.col_name == "") {
            copy_cond.lhs_col.col_name = rec_cols[0].name;
        }
        Value lhs_val = get_aggr_value(rec_cols, rec, copy_cond.lhs_col, cond.lhs_col.aggregate);
        Value rhs_val;
        if (cond.is_rhs_val) {
            rhs_val = cond.rhs_val;
        } else {
            if (copy_cond.rhs_col.col_name == "") {
                copy_cond.rhs_col.col_name = rec_cols[0].name;
            }
            rhs_val = get_aggr_value(rec_cols, rec, copy_cond.rhs_col, cond.rhs_col.aggregate);
        }
        return check_cond(lhs_val, rhs_val, cond.op);
    }

    bool eval_aggr_conds(const std::vector<ColMeta> &rec_cols, const std::vector<Condition>& conds, std::vector<std::unique_ptr<RmRecord>>& records) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition &cond) { return eval_aggr_cond(rec_cols, cond, records); });
    }
};