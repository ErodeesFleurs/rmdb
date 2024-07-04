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

extern int count_seq_scan;

class SeqScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;              // 表的名称
    std::vector<Condition> conds_;      // scan的条件
    RmFileHandle* fh_;                  // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // scan后生成的记录的字段
    size_t len_;                        // scan后生成的每条记录的长度
    std::vector<Condition> fed_conds_;  // 同conds_，两个字段相同

    Rid rid_;
    std::unique_ptr<RecScan> scan_;  // table_iterator

    SmManager* sm_manager_;

   public:
    SeqScanExecutor(SmManager* sm_manager, std::string tab_name,
                    std::vector<Condition> conds, Context* context) {
        sm_manager_ = sm_manager;
        tab_name_ = std::move(tab_name);
        conds_ = std::move(conds);
        TabMeta& tab = sm_manager_->db_.get_table(tab_name_);
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab.cols;
        len_ = cols_.back().offset + cols_.back().len;

        fed_conds_ = conds_;

        context_ = context;
        if (context_->txn_ != nullptr) {
            context_->lock_mgr_->lock_shared_on_table(
                context_->txn_, sm_manager_->fhs_[tab_name_]->GetFd());
        }
    }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta>& cols() const override { return cols_; }

    void beginTuple() override {
        // auto delay = [](volatile int64_t count) {
        //     for (volatile int64_t i = 0; i < count; i++) {}
        // };
        // delay(5000000);

        scan_ = std::make_unique<RmScan>(fh_);
        while (!scan_->is_end()) {  // 从头开始扫描
            rid_ = scan_->rid();
            count_seq_scan++;
            auto rec = fh_->get_record(rid_, context_);
            if (fed_conds_.empty() ||
                eval_conds(cols_, fed_conds_, rec.get())) {
                break;
            }
            scan_->next();
        }
    }

    void nextTuple() override {
        if (!scan_->is_end()) {
            scan_->next();
        }
        while (!scan_->is_end()) {
            count_seq_scan++;
            rid_ = scan_->rid();
            auto rec = fh_->get_record(rid_, context_);
            if (fed_conds_.empty() ||
                eval_conds(cols_, fed_conds_, rec.get())) {
                break;
            }
            scan_->next();
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        return fh_->get_record(rid_, context_);
    }

    bool is_end() const override { return scan_->is_end(); }

    Rid& rid() override { return rid_; }

    ExecutorType getType() const override { return ExecutorType::SEQ_SCAN; }
};