#pragma once
#include <climits>
#include <numeric>
#include <unordered_map>
#include <vector>
#include "execution_defs.h"
#include "execution_group.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

class AggregateExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;
    std::vector<ColMeta> cols_;
    std::vector<ColMeta> output_cols_;
    std::vector<AggregateType> agg_types_;
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>>*
        grouped_records_;
    std::vector<std::unique_ptr<RmRecord>> aggregated_records_;
    std::vector<std::unique_ptr<RmRecord>>::iterator current_record_;

   public:
    AggregateExecutor(std::unique_ptr<AbstractExecutor> prev,
                      const std::vector<TabCol>& sel_cols,
                      const std::vector<AggregateType>& agg_types)
        : prev_(std::move(prev)), agg_types_(agg_types) {
        std::cerr << "AggregateExecutor" << std::endl;
        // 构造输出列
        for (const auto& sel_col : sel_cols) {
            if (sel_col.col_name == "*" &&
                sel_col.aggregate == AggregateType::COUNT) {
                cols_.push_back(ColMeta{.tab_name = "",
                                        .name = "*",
                                        .type = TYPE_INT,
                                        .len = sizeof(int),
                                        .offset = 0});
            } else {
                cols_.push_back(*prev_->get_col(prev_->cols(), sel_col));
            }
            output_cols_.push_back(cols_.back());
        }
        // 如果首位是COUNT
        output_cols_.front().offset = 0;
        if (agg_types[0] == AggregateType::COUNT) {
            output_cols_.front().type = TYPE_INT;
            output_cols_.front().len = sizeof(int);
        }
        for (size_t i = 1; i < output_cols_.size(); ++i) {
            if (agg_types[i] == AggregateType::COUNT) {
                output_cols_[i].type = TYPE_INT;
                output_cols_[i].len = sizeof(int);
            }
            output_cols_[i].offset =
                output_cols_[i - 1].offset + output_cols_[i - 1].len;
        }
    }

    void beginTuple() override {
        std::cerr << "Aggregate BeginTuple" << std::endl;
        prev_->beginTuple();
        if (auto group_executor = dynamic_cast<GroupExecutor*>(prev_.get())) {
            for (const auto& group : group_executor->group_iterators) {
                auto record = aggregateGroup(group->second);
                if (record) {
                    aggregated_records_.push_back(std::move(record));
                }
            }
        } else {
            std::vector<std::unique_ptr<RmRecord>> records;
            while (!prev_->is_end()) {
                records.push_back(prev_->Next());
                prev_->nextTuple();
            }
            auto record = aggregateGroup(records);
            if (record) {
                aggregated_records_.push_back(std::move(record));
            }
        }
        current_record_ = aggregated_records_.begin();
    }

    void nextTuple() override {
        if (current_record_ != aggregated_records_.end()) {
            ++current_record_;
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        return std::make_unique<RmRecord>(**current_record_);
    }

    bool is_end() const override {
        return current_record_ == aggregated_records_.end();
    }

    const std::vector<ColMeta>& cols() const override { return output_cols_; }

    Rid& rid() override { return _abstract_rid; }

    ExecutorType getType() const override { return ExecutorType::AGGREGATE; }

    std::unique_ptr<RmRecord> aggregateGroup(
        const std::vector<std::unique_ptr<RmRecord>>& records) {
        bool is_count = true;
        for (const auto& agg_type : agg_types_) {
            if (agg_type != AggregateType::COUNT) {
                is_count = false;
                break;
            }
        }
        // 如果没有记录，且不是count，返回空
        if (records.empty() && !is_count)
            return nullptr;
        // 如果没有记录，且是count，返回0
        else if (records.empty() && is_count) {
            auto result = std::make_unique<RmRecord>();
            for (size_t i = 0; i < agg_types_.size(); ++i) {
                auto count_value = Value();
                count_value.set_int(0);
                count_value.init_raw();
                result->append(count_value.raw->data, count_value.raw->size);
            }
            return result;
        }
        auto result = std::make_unique<RmRecord>();
        for (size_t i = 0; i < agg_types_.size(); ++i) {
            std::cerr << "Aggregating: " << aggregate2str(agg_types_[i])
                      << std::endl;
            std::cerr << "Aggregating: " << cols_[i].name << " "
                      << cols_[i].tab_name << std::endl;
            Value res = get_aggr_value(cols_, records,
                                       TabCol{.tab_name = cols_[i].tab_name,
                                              .col_name = cols_[i].name},
                                       agg_types_[i]);
            std::cerr << "Aggregated: " << res << std::endl;
            res.init_raw();
            result->append(res.raw->data, res.raw->size);
        }
        std::cerr << "result size: " << result->size << std::endl;
        return result;
    }
};