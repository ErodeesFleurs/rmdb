#pragma once
#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"
#include "execution_group.h" // Include the header file that contains the definition of GroupExecutor
#include <unordered_map>
#include <vector>



class AggregateExecutor : public AbstractExecutor {
private:
    std::unique_ptr<AbstractExecutor> prev_;
    std::vector<ColMeta> cols_;
    std::vector<AggregateType> agg_types_;
    bool is_grouped = false;
    bool is_end_ = false;
    std::unordered_map<std::string, std::vector<std::unique_ptr<RmRecord>>> grouped_records_;
    std::vector<std::unique_ptr<RmRecord>> aggregated_records_;
    std::vector<std::unique_ptr<RmRecord>>::iterator current_record_;

public:
    AggregateExecutor(std::unique_ptr<AbstractExecutor> prev, const std::vector<TabCol>& sel_cols, const std::vector<AggregateType>& agg_types)
        : prev_(std::move(prev)), agg_types_(agg_types) {
        for (const auto& sel_col : sel_cols) {
            cols_.push_back(*prev_->get_col(prev_->cols(), sel_col));
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
        prev_->beginTuple();
        if (is_grouped) {
            grouped_records_ = dynamic_cast<GroupExecutor*>(prev_.get())->cloneGroupedRecords();
        }
        else {
            aggregated_records_.emplace_back(prev_->Next());
        }
    }

    void nextTuple() override {

    }

    std::unique_ptr<RmRecord> Next() override {
        return nullptr;
    }

    bool is_end() const override {
        return true;
    }

    const std::vector<ColMeta>& cols() const override {
        return cols_;
    }

    Rid& rid() override {
        return _abstract_rid;
    }
};
