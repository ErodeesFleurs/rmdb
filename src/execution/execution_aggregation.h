#pragma once
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"
#include "execution_group.h"
#include <unordered_map>
#include <numeric>
#include <vector>
#include <climits>


class AggregateExecutor : public AbstractExecutor {
private:
    std::unique_ptr<AbstractExecutor> prev_;
    std::vector<ColMeta> cols_;
    std::vector<ColMeta> output_cols_;
    std::vector<AggregateType> agg_types_;
    bool is_grouped = false;
    bool is_end_ = false;
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> grouped_records_;
    std::vector<std::unique_ptr<RmRecord>> aggregated_records_;
    std::vector<std::unique_ptr<RmRecord>>::iterator current_record_;

public:
    AggregateExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol>& sel_cols, const std::vector<AggregateType>& agg_types)
        : prev_(std::move(prev)), agg_types_(agg_types) {
        std::cerr << "sel_cols size: " << sel_cols.size() << std::endl;
        std::cerr << "agg_types size: " << agg_types.size() << std::endl;
        for (const auto& sel_col : sel_cols) {
            cols_.push_back(*prev_->get_col(prev_->cols(), sel_col));
            std::cerr << "Sel col: " << cols_.back().name << " " << cols_.back().tab_name << " " << cols_.back().offset << " " << cols_.back().type << std::endl;
            output_cols_.push_back(cols_.back());
        }
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
            output_cols_[i].offset = output_cols_[i - 1].offset + output_cols_[i - 1].len;
        }
        auto pointer = prev_.get();
        if (dynamic_cast<GroupExecutor*>(pointer)) {
            is_grouped = true;
        }
        for (const auto& agg_type : agg_types) {
            std::cerr << "Agg type: " << agg_type << std::endl;
        }
    }

    void beginTuple() override {
        std::cerr << "Aggregate BeginTuple" << " " << is_grouped << std::endl;
        prev_->beginTuple();
        if (is_grouped) {
            grouped_records_ = dynamic_cast<GroupExecutor*>(prev_.get())->cloneGroupedRecords();
            for (const auto& group : grouped_records_) {
                std::cerr << "Group size: " << group.second.size() << std::endl;
            }
            for (const auto& group : grouped_records_) {
                aggregated_records_.push_back(aggregateGroup(group.second));
            }
        }
        else {
            std::vector<std::unique_ptr<RmRecord>> records;
            while (!prev_->is_end()) {
                records.push_back(prev_->Next());
                prev_->nextTuple();
            }
            aggregated_records_.push_back(aggregateGroup(records));
            std::cerr << "Aggregated records size: " << aggregated_records_.size() << std::endl;
        }
        current_record_ = aggregated_records_.begin();
    }

    void nextTuple() override {
        if (current_record_ != aggregated_records_.end()) {
            ++current_record_;
        }
        is_end_ = (current_record_ == aggregated_records_.end());
    }

    std::unique_ptr<RmRecord> Next() override {
        std::cerr << "Aggregate Next " << is_end_ << std::endl;
        if (is_end_) {
            return nullptr;
        }
        return std::make_unique<RmRecord>(**current_record_);
    }

    bool is_end() const override {
        return is_end_;
    }

    const std::vector<ColMeta>& cols() const override {
        return output_cols_;
    }

    Rid& rid() override {
        return _abstract_rid;
    }

    std::unique_ptr<RmRecord> aggregateGroup(const std::vector<std::unique_ptr<RmRecord>>& records) {
        bool is_count = true;
        for (const auto& agg_type : agg_types_) {
            if (agg_type != AggregateType::COUNT) {
                is_count = false;
                break;
            }
        }
        if (records.empty() && !is_count) return nullptr;
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
            std::cerr << "Aggregating: " << aggregate2str(agg_types_[i]) << std::endl;
            switch (agg_types_[i]) {
                case AggregateType::NONE: {
                    std::cerr << "No aggregation" << std::endl;
                    auto col = cols_[i];
                    result->append(records[0]->data + col.offset, col.len);
                    break;
                }
                case AggregateType::SUM: {
                    std::cerr << "Summing" << std::endl;
                    auto sum_value = Value();
                    if (cols_[i].type == TYPE_INT) {
                        sum_value.set_int(0);
                    }
                    else if (cols_[i].type == TYPE_FLOAT) {
                        sum_value.set_float(0);
                    }
                    for (const auto& record : records) {
                        auto col = cols_[i];
                        auto value = get_value(col.type, record->data + col.offset);
                        sum_value = sum_value + value;
                    };
                    sum_value.init_raw();
                    std::cerr << "Sum value: " << sum_value << std::endl;
                    result->append(sum_value.raw->data, sum_value.raw->size);
                    break;
                }
                case AggregateType::COUNT: {
                    std::cerr << "Counting" << std::endl;
                    auto count_value = Value();
                    count_value.set_int(records.size());
                    count_value.init_raw();
                    std::cerr << "Count value: " << count_value << std::endl;
                    result->append(count_value.raw->data, count_value.raw->size);
                    break;
                }
                case AggregateType::MAX: {
                    std::cerr << "Maxing" << std::endl;
                    auto max_value = Value();
                    if (cols_[i].type == TYPE_INT) {
                        max_value.set_int(INT_MIN);
                    }
                    else if (cols_[i].type == TYPE_FLOAT) {
                        max_value.set_float(__DBL_MIN__);
                    }
                    else if (cols_[i].type == TYPE_STRING) {
                        max_value.set_str("");
                    }
                    for (const auto& record : records) {
                        auto col = cols_[i];
                        auto value = get_value(col.type, record->data + col.offset);
                        max_value = std::max(max_value, value);
                    };
                    max_value.init_raw();
                    std::cerr << "Max value: " << max_value << std::endl;
                    result->append(max_value.raw->data, max_value.raw->size);
                    break;
                }
                case AggregateType::MIN: {
                    std::cerr << "Minnig" << std::endl;
                    auto min_value = Value();
                    if (cols_[i].type == TYPE_INT) {
                        min_value.set_int(INT_MAX);
                    }
                    else if (cols_[i].type == TYPE_FLOAT) {
                        min_value.set_float(__DBL_MAX__);
                    }
                    else if (cols_[i].type == TYPE_STRING) {
                        min_value.set_str(std::string(255, 255));
                    }
                    for (const auto& record : records) {
                        auto col = cols_[i];
                        auto value = get_value(col.type, record->data + col.offset);
                        min_value = std::min(min_value, value);
                    };
                    min_value.init_raw();
                    std::cerr << "Min value: " << min_value << std::endl;
                    result->append(min_value.raw->data, min_value.raw->size);
                    break;
                }
            }
        }
        return result;
    }
};