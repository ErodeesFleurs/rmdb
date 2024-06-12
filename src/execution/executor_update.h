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

class UpdateExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;
    std::vector<Condition> conds_;
    RmFileHandle *fh_;
    std::vector<Rid> rids_;
    std::string tab_name_;
    std::vector<SetClause> set_clauses_;
    SmManager *sm_manager_;

   public:
    UpdateExecutor(SmManager *sm_manager, const std::string &tab_name, std::vector<SetClause> set_clauses,
                   std::vector<Condition> conds, std::vector<Rid> rids, Context *context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        set_clauses_ = set_clauses;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;
    }
    std::unique_ptr<RmRecord> Next() override {
        std::unordered_map<TabCol, ColMeta, std::hash<TabCol>, std::equal_to<TabCol>, std::allocator<std::pair<const TabCol, ColMeta>>> col_metas(0);
        for (const auto &set_clause : set_clauses_) {
            col_metas[set_clause.lhs] = *get_col(tab_.cols, set_clause.lhs);
        }
        for (const auto &rid : rids_) {
            std::unique_ptr<RmRecord> record = fh_->get_record(rid, context_);
            for (const auto &set_clause : set_clauses_) {
                auto &col_meta = col_metas[set_clause.lhs];
                auto value = set_clause.rhs;
                value.init_raw(col_meta.len);
                memcmp(record->data + col_meta.offset, value.raw->data, col_meta.len);
            }
            fh_->update_record(rid, record->data, context_);
        }
        return nullptr;
    }

    Rid &rid() override { return _abstract_rid; }
};