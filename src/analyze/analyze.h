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

#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "common/common.h"
#include "optimizer/optimizer.h"
#include "parser/parser.h"
#include "portal.h"
#include "system/sm.h"
class Analyze {
   private:
    SmManager* sm_manager_;
    Optimizer* optimizer_;
    QlManager* ql_manager_;
    Portal* portal_;

   public:
    Analyze(SmManager* sm_manager, Optimizer* optimizer, QlManager* ql_manager,
            Portal* portal)
        : sm_manager_(sm_manager),
          optimizer_(optimizer),
          ql_manager_(ql_manager),
          portal_(portal) {}
    ~Analyze() {}
    std::shared_ptr<Query> do_analyze(std::shared_ptr<ast::TreeNode> root,
                                      Context* context);

   private:
    TabCol check_column(const std::vector<ColMeta>& all_cols, TabCol target);
    void get_all_cols(const std::vector<std::string>& tab_names,
                      std::vector<ColMeta>& all_cols);
    void get_clause(
        const std::vector<std::shared_ptr<ast::BinaryExpr>>& sv_conds,
        std::vector<Condition>& conds, Context* context);
    void set_clause(
        const std::string& tab_name,
        const std::vector<std::shared_ptr<ast::SetClause>>& sv_conds,
        std::vector<SetClause>& conds);
    void check_clause(const std::vector<std::string>& tab_names,
                      std::vector<Condition>& conds);
    void check_col_group_and_aggr(const std::vector<TabCol>& cols,
                                  const std::vector<TabCol>& group_cols);
    void check_conds_with_aggregate(const std::vector<Condition>& conds);
    void check_having_conds(const std::vector<Condition>& having_conds,
                            const std::vector<TabCol>& group_cols);
    void check_group(const std::vector<TabCol>& group_cols,
                     const std::vector<std::string>& tab_names);
    void check_without_group(const std::vector<TabCol>& group_cols,
                             const std::vector<Condition>& having_conds);
    Value convert_sv_value(const std::shared_ptr<ast::Value>& sv_val);
    CompOp convert_sv_comp_op(ast::SvCompOp op);
};
