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
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> grouped_records;
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator> group_iterators;
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator>::iterator current_group;
    std::unique_ptr<RmRecord> current_tuple;

   public:
    GroupExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol>& sel_cols, const std::vector<TabCol>& group_cols) {
        prev_ = std::move(prev);
        for (const auto& sel_col : sel_cols) {
            cols_.push_back(*prev_->get_col(prev_->cols(), sel_col));
        }
        group_cols_ = group_cols;
    }

    void beginTuple() override {
        std::cerr << "Group BeginTuple" << std::endl;
        prev_->beginTuple();
        grouped_records.clear();
        while (!prev_->is_end()) {
            auto tuple = prev_->Next();
            std::string group_key = generateGroupKey(tuple);
            grouped_records[group_key].emplace_back(std::move(tuple));
            prev_->nextTuple();
        }
        std::cerr << "Grouped records: " << grouped_records.size() << std::endl;
        group_iterators.clear();
        for (auto it = grouped_records.begin(); it != grouped_records.end(); ++it) {
            group_iterators.push_back(it);
        }
        current_group = group_iterators.begin();
        if (current_group != group_iterators.end()) {
            current_tuple = std::move(current_group->operator->()->second.front());
        }
    }

    void nextTuple() override {
        if (current_group != group_iterators.end()) {
            current_group++;
            if (current_group != group_iterators.end()) {
                current_tuple = std::move(current_group->operator->()->second.front());
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

    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> cloneGroupedRecords() {
        std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> cloned_records;
        for (const auto& group : grouped_records) {
            std::vector<std::unique_ptr<RmRecord>> cloned_group;
            for (const auto& record : group.second) {
                cloned_group.emplace_back(std::make_unique<RmRecord>(*record));
            }
            cloned_records[group.first] = std::move(cloned_group);
        }
        return cloned_records;
    }

   private:

   std::string generateGroupKey(std::unique_ptr<RmRecord>& record) {
        std::string group_key;
        for (const auto& group_col : group_cols_) {
            const auto col_meta = prev_->get_col(prev_->cols(), group_col);
            const char* col_data = record->data + col_meta->offset;
            if (col_meta->type == TYPE_INT) {
                group_key += std::to_string(*(int*)col_data);
            } else if (col_meta->type == TYPE_FLOAT) {
                group_key += std::to_string(*(double*)col_data);
            } else {
                group_key += std::string(col_data, col_meta->len);
            }
            group_key += '|';
        }
        std::cerr << "Group key: " << group_key << std::endl;
        return group_key;
    }
};