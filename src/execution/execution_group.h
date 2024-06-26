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
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator> group_iterators;
    std::vector<std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>::iterator>::iterator current_group;
    std::unique_ptr<RmRecord> current_tuple;

   public:
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> grouped_records;

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

        while (!prev_->is_end()) {
            auto tuple = prev_->Next();
            std::string group_key = generateGroupKey(tuple);
            grouped_records[group_key].emplace_back(std::move(tuple));
            prev_->nextTuple();
        }
        // 条件过滤
        std::vector<std::string> group_keys_to_remove;
        for (auto& [key, records] : grouped_records) {
            std::cerr << "Group key: " << key << " " << records.size() << std::endl;
            if (having_conds_.empty()) {
                break;
            }
            bool should_remove = false;
            should_remove = !eval_aggr_conds(cols_, having_conds_, records);
            if (should_remove) {
                group_keys_to_remove.push_back(key);
            }
        }
        for (const auto& key : group_keys_to_remove) {
            grouped_records.erase(key);
        }
        group_iterators.clear();
        for (auto it = grouped_records.begin(); it != grouped_records.end(); ++it) {
            group_iterators.push_back(it);
        }
        current_group = group_iterators.begin();
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
        // std::cerr << "Group key: " << group_key << std::endl;
        return group_key;
    }

    Value get_aggr_value(const std::vector<ColMeta>& rec_cols, std::vector<std::unique_ptr<RmRecord>>& rec, const std::string &col_name, AggregateType agg_type) {
        Value val;
        auto pos = std::find_if(rec_cols.begin(), rec_cols.end(), [&](const ColMeta &col) {
            return col.name == col_name;
        });
        if (pos == rec_cols.end()) {
            throw ColumnNotFoundError(col_name);
        }
        auto col_meta = *pos;
        if (agg_type == AggregateType::COUNT) {
            val.set_int(rec.size());
        } else if (agg_type == AggregateType::SUM) {
            if (col_meta.type == TYPE_INT) {
                int sum = 0;
                for (const auto& record : rec) {
                    sum += *(int*)(record->data + col_meta.offset);
                }
                val.set_int(sum);
            } else if (col_meta.type == TYPE_FLOAT) {
                double sum = 0;
                for (const auto& record : rec) {
                    sum += *(double*)(record->data + col_meta.offset);
                }
                val.set_float(sum);
            }
        } else if (agg_type == AggregateType::MAX) {
            if (col_meta.type == TYPE_INT) {
                int max = std::numeric_limits<int>::min();
                for (const auto& record : rec) {
                    max = std::max(max, *(int*)(record->data + col_meta.offset));
                }
                val.set_int(max);
            } else if (col_meta.type == TYPE_FLOAT) {
                double max = std::numeric_limits<double>::min();
                for (const auto& record : rec) {
                    max = std::max(max, *(double*)(record->data + col_meta.offset));
                }
                val.set_float(max);
            } else if (col_meta.type == TYPE_STRING) {
                std::string max = "";
                for (const auto& record : rec) {
                    std::string str(record->data + col_meta.offset, col_meta.len);
                    max = std::max(max, str);
                }
                val.set_str(max);
            }
        } else if (agg_type == AggregateType::MIN) {
            if (col_meta.type == TYPE_INT) {
                int min = std::numeric_limits<int>::max();
                for (const auto& record : rec) {
                    min = std::min(min, *(int*)(record->data + col_meta.offset));
                }
                val.set_int(min);
            }
            else if (col_meta.type == TYPE_FLOAT) {
                double min = std::numeric_limits<double>::max();
                for (const auto& record : rec) {
                    min = std::min(min, *(double*)(record->data + col_meta.offset));
                }
                val.set_float(min);
            }
            else if (col_meta.type == TYPE_STRING) {
                std::string min = std::string(255, 255);
                for (const auto& record : rec) {
                    std::string str(record->data + col_meta.offset, col_meta.len);
                    min = std::min(min, str);
                }
                val.set_str(min);
            }
        }
        return val;
    }

    bool eval_aggr_cond(const std::vector<ColMeta>& rec_cols, const Condition& cond, std::vector<std::unique_ptr<RmRecord>>& rec) {
        auto copy_cond = cond;
        if (copy_cond.lhs_col.col_name == "") {
            copy_cond.lhs_col.col_name = rec_cols[0].name;
        }
        Value lhs_val = get_aggr_value(rec_cols, rec, copy_cond.lhs_col.col_name, cond.lhs_col.aggregate);
        Value rhs_val;
        if (cond.is_rhs_val) {
            rhs_val = cond.rhs_val;
        } else {
            if (copy_cond.rhs_col.col_name == "") {
                copy_cond.rhs_col.col_name = rec_cols[0].name;
            }
            rhs_val = get_aggr_value(rec_cols, rec, copy_cond.rhs_col.col_name, cond.rhs_col.aggregate);
        }
        
        std::cerr << "lhs_val: " << lhs_val << std::endl;
        std::cerr << "rhs_val: " << rhs_val << std::endl;
        return check_cond(lhs_val, rhs_val, cond.op);
    }

    bool eval_aggr_conds(const std::vector<ColMeta> &rec_cols, const std::vector<Condition>& conds, std::vector<std::unique_ptr<RmRecord>>& records) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition &cond) { return eval_aggr_cond(rec_cols, cond, records); });
    }
};