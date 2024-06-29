/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "analyze.h"

/**
 * @description: 分析器，进行语义分析和查询重写，需要检查不符合语义规定的部分
 * @param {shared_ptr<ast::TreeNode>} parse parser生成的结果集
 * @return {shared_ptr<Query>} Query 
 */
std::shared_ptr<Query> Analyze::do_analyze(
    std::shared_ptr<ast::TreeNode> parse) {
    std::shared_ptr<Query> query = std::make_shared<Query>();
    if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(parse)) {
        // 处理表名
        query->tables = std::move(x->tabs);
        /** TODO: 检查表是否存在 */
        for (const auto& table : query->tables) {
            if (!sm_manager_->db_.is_table(table)) {
                throw TableNotFoundError(table);
            }
        }

        std::vector<ColMeta> all_cols;
        get_all_cols(query->tables, all_cols);

        // 处理target list，再target list中添加上表名，例如 a.id
        for (auto& sv_sel_col : x->cols) {
            TabCol sel_col = {.tab_name = sv_sel_col->tab_name,
                              .col_name = sv_sel_col->col_name,
                              .as_name = sv_sel_col->as_name,
                              .aggregate = sv_sel_col->aggregate};
            query->cols.push_back(sel_col);
        }

        // 处理group by
        for (auto& sv_group_col : x->group) {
            TabCol group_col = {.tab_name = sv_group_col->cols->tab_name,
                                .col_name = sv_group_col->cols->col_name,
                                .as_name = sv_group_col->cols->as_name,
                                .aggregate = sv_group_col->cols->aggregate};
            query->group_cols.push_back(group_col);
        }
        // 如果有group by，检查group by的列是否存在
        for (auto& group_col : query->group_cols) {
            group_col = check_column(all_cols, group_col);  // Group列元数据校验
        }
        check_group(query->group_cols, query->tables);

        if (query->cols.empty()) {
            // select all columns
            for (auto& col : all_cols) {
                TabCol sel_col = {.tab_name = col.tab_name,
                                  .col_name = col.name,
                                  .as_name = col.name,
                                  .aggregate = AggregateType::NONE};
                query->cols.push_back(sel_col);
            }
        } else {
            // infer table name from column name
            for (auto& sel_col : query->cols) {
                // 如果是count(*)，则默认为第一个列
                if (sel_col.aggregate == AggregateType::COUNT &&
                    sel_col.col_name.empty()) {
                    sel_col.col_name = all_cols[0].name;
                    sel_col.tab_name = all_cols[0].tab_name;
                }
                sel_col = check_column(all_cols, sel_col);  // 列元数据校验
            }
        }
        // 处理where条件
        get_clause(x->conds, query->conds);
        check_clause(query->tables, query->conds);
        // 处理having条件
        get_clause(x->having_conds, query->having_conds);
        check_clause(query->tables, query->having_conds);
        // 检查where条件中是否有聚合函数
        check_conds_with_aggregate(query->conds);
        // 检查having条件中是否有不是聚合函数也不是group by的列
        check_having_conds(query->having_conds, query->group_cols);
        // 检查group by和select中的列是否符合规范
        check_col_group_and_aggr(query->cols, query->group_cols);
        // 检测group by不存在时，是否有having条件
        check_without_group(query->group_cols, query->having_conds);
    } else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(parse)) {
        /** TODO: */
        set_clause(x->tab_name, x->set_clauses, query->set_clauses);
        get_clause(x->conds, query->conds);
        check_clause({x->tab_name}, query->conds);
    } else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(parse)) {
        //处理where条件
        get_clause(x->conds, query->conds);
        check_clause({x->tab_name}, query->conds);
    } else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(parse)) {
        // 处理insert 的values值
        for (auto& sv_val : x->vals) {
            query->values.push_back(convert_sv_value(sv_val));
        }
    } else {
        // do nothing
    }
    query->parse = std::move(parse);
    return query;
}

TabCol Analyze::check_column(const std::vector<ColMeta>& all_cols,
                             TabCol target) {
    if (target.tab_name.empty()) {
        // Table name not specified, infer table name from column name
        std::string tab_name;
        for (auto& col : all_cols) {
            if (col.name == target.col_name) {
                if (!tab_name.empty()) {
                    throw AmbiguousColumnError(target.col_name);
                }
                tab_name = col.tab_name;
            }
        }
        if (tab_name.empty()) {
            if (target.col_name == "*" &&
                target.aggregate == AggregateType::COUNT) {
                return target;
            }
            throw ColumnNotFoundError(target.col_name);
        }
        target.tab_name = tab_name;
    } else {
        /** TODO: Make sure target column exists */
        int count = 0;
        for (auto& col : all_cols) {  // 遍历查找是否存在以及是否重复
            if (col.name == target.col_name &&
                col.tab_name == target.tab_name) {
                count++;
                if (count > 1) {
                    throw AmbiguousColumnError(target.col_name);
                }
            }
        }
        if (count == 0) {  // 如果未能找到
            throw ColumnNotFoundError(target.col_name);
        }
    }
    return target;
}

void Analyze::get_all_cols(const std::vector<std::string>& tab_names,
                           std::vector<ColMeta>& all_cols) {
    for (auto& sel_tab_name : tab_names) {
        // 这里db_不能写成get_db(), 注意要传指针
        const auto& sel_tab_cols =
            sm_manager_->db_.get_table(sel_tab_name).cols;
        all_cols.insert(all_cols.end(), sel_tab_cols.begin(),
                        sel_tab_cols.end());
    }
}

void Analyze::get_clause(
    const std::vector<std::shared_ptr<ast::BinaryExpr>>& sv_conds,
    std::vector<Condition>& conds) {
    conds.clear();
    for (auto& expr : sv_conds) {
        Condition cond;
        cond.lhs_col = {.tab_name = expr->lhs->tab_name,
                        .col_name = expr->lhs->col_name,
                        .as_name = expr->lhs->as_name,
                        .aggregate = expr->lhs->aggregate};
        cond.op = convert_sv_comp_op(expr->op);
        if (auto rhs_val = std::dynamic_pointer_cast<ast::Value>(expr->rhs)) {
            cond.is_rhs_val = true;
            cond.rhs_val = convert_sv_value(rhs_val);
        } else if (auto rhs_col =
                       std::dynamic_pointer_cast<ast::Col>(expr->rhs)) {
            cond.is_rhs_val = false;
            cond.rhs_col = {.tab_name = rhs_col->tab_name,
                            .col_name = rhs_col->col_name,
                            .as_name = rhs_col->as_name,
                            .aggregate = rhs_col->aggregate};
        }
        conds.push_back(cond);
    }
}

void Analyze::set_clause(
    const std::string& tab_name,
    const std::vector<std::shared_ptr<ast::SetClause>>& sv_conds,
    std::vector<SetClause>& conds) {
    conds.clear();
    for (auto& expr : sv_conds) {
        SetClause cond;
        switch (expr->setOp) {
            case ast::SvSetOp::SV_OP_SET: {
                cond.op = OP_SET;
                break;
            }
            case ast::SvSetOp::SV_OP_ADD: {
                cond.op = OP_ADD;
                break;
            }
            case ast::SvSetOp::SV_OP_SUB: {
                cond.op = OP_SUB;
                break;
            }
        }
        cond.lhs = {.tab_name = tab_name, .col_name = expr->col_name};
        cond.rhs = convert_sv_value(expr->val);
        conds.push_back(cond);
    }
}

void Analyze::check_clause(const std::vector<std::string>& tab_names,
                           std::vector<Condition>& conds) {
    // auto all_cols = get_all_cols(tab_names);
    std::vector<ColMeta> all_cols;
    get_all_cols(tab_names, all_cols);
    // Get raw values in where clause
    for (auto& cond : conds) {
        // Infer table name from column name
        cond.lhs_col = check_column(all_cols, cond.lhs_col);
        if (!cond.is_rhs_val) {
            cond.rhs_col = check_column(all_cols, cond.rhs_col);
        }
        // 如果是count(*)，则不需要检查类型
        if (cond.lhs_col.aggregate == AggregateType::COUNT &&
            cond.lhs_col.col_name == "*") {
            ColType lhs_type = TYPE_INT;
            ColType rhs_type;
            if (cond.is_rhs_val) {
                rhs_type = cond.rhs_val.type;
                cond.rhs_val.init_raw();
            } else {
                rhs_type = sm_manager_->db_.get_table(cond.rhs_col.tab_name)
                               .get_col(cond.rhs_col.col_name)
                               ->type;
            }
            if (lhs_type != rhs_type &&
                (lhs_type == TYPE_STRING || rhs_type == TYPE_STRING)) {
                throw IncompatibleTypeError(coltype2str(lhs_type),
                                            coltype2str(rhs_type));
            }
            continue;
        }
        TabMeta& lhs_tab = sm_manager_->db_.get_table(cond.lhs_col.tab_name);
        auto lhs_col = lhs_tab.get_col(cond.lhs_col.col_name);
        ColType lhs_type = lhs_col->type;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            if (rhs_type == TYPE_FLOAT) {
                cond.rhs_val.init_raw(sizeof(double));
            } else if (rhs_type == TYPE_INT) {
                cond.rhs_val.init_raw(sizeof(int));
            } else {
                cond.rhs_val.init_raw(lhs_col->len);
            }
        } else {
            TabMeta& rhs_tab =
                sm_manager_->db_.get_table(cond.rhs_col.tab_name);
            auto rhs_col = rhs_tab.get_col(cond.rhs_col.col_name);
            rhs_type = rhs_col->type;
        }
        if (lhs_type != rhs_type &&
            (lhs_type == TYPE_STRING || rhs_type == TYPE_STRING)) {
            throw IncompatibleTypeError(coltype2str(lhs_type),
                                        coltype2str(rhs_type));
        }
    }
}

void Analyze::check_col_group_and_aggr(const std::vector<TabCol>& cols,
                                       const std::vector<TabCol>& group_cols) {
    if (group_cols.empty()) {
        bool has_aggr =
            std::any_of(cols.begin(), cols.end(), [](const TabCol& col) {
                return col.aggregate != AggregateType::NONE;
            });
        bool has_non_aggr =
            std::any_of(cols.begin(), cols.end(), [](const TabCol& col) {
                return col.aggregate == AggregateType::NONE;
            });
        if (has_aggr && has_non_aggr) {
            throw RMDBError(
                "Non aggregate column in select list with aggregate column");
        }
    } else {
        for (auto& col : cols) {
            if (col.aggregate != AggregateType::NONE) {
                continue;
            }
            bool found =
                std::any_of(group_cols.begin(), group_cols.end(),
                            [&](const TabCol& group_col) {
                                if (col.col_name == group_col.col_name) {
                                    return true;
                                }
                                return false;
                            });
            if (!found) {
                throw RMDBError("Non aggregate column not in group by");
            }
        }
    }
}

void Analyze::check_conds_with_aggregate(const std::vector<Condition>& conds) {
    for (auto& cond : conds) {
        if (cond.lhs_col.aggregate != AggregateType::NONE ||
            (!cond.is_rhs_val &&
             cond.rhs_col.aggregate != AggregateType::NONE)) {
            throw RMDBError("Aggregate column in where clause");
        }
    }
}

void Analyze::check_having_conds(const std::vector<Condition>& having_conds,
                                 const std::vector<TabCol>& group_cols) {
    for (auto& cond : having_conds) {
        if (cond.lhs_col.aggregate == AggregateType::NONE) {
            bool found = std::any_of(
                group_cols.begin(), group_cols.end(),
                [&](const TabCol& group_col) {
                    if (cond.lhs_col.col_name == group_col.col_name &&
                        cond.lhs_col.tab_name == group_col.tab_name) {
                        return true;
                    }
                    return false;
                });
            if (!found) {
                throw RMDBError("Non aggregate column not in group by");
            }
        }
        if (!cond.is_rhs_val && cond.rhs_col.aggregate == AggregateType::NONE) {
            bool found = std::any_of(
                group_cols.begin(), group_cols.end(),
                [&](const TabCol& group_col) {
                    if (cond.rhs_col.col_name == group_col.col_name &&
                        cond.rhs_col.tab_name == group_col.tab_name) {
                        return true;
                    }
                    return false;
                });
            if (!found) {
                throw RMDBError("Non aggregate column not in group by");
            }
        }
    }
}

void Analyze::check_group(const std::vector<TabCol>& group_cols,
                          const std::vector<std::string>& tab_names) {
    std::vector<ColMeta> all_cols;
    get_all_cols(tab_names, all_cols);
    for (auto& group_col : group_cols) {
        bool found = false;
        for (auto& col : all_cols) {
            if (col.name == group_col.col_name &&
                col.tab_name == group_col.tab_name) {
                found = true;
                break;
            }
        }
        if (!found) {
            throw RMDBError("Group by column not found");
        }
    }
}

void Analyze::check_without_group(const std::vector<TabCol>& group_cols,
                                  const std::vector<Condition>& having_conds) {
    if (group_cols.empty() && !having_conds.empty()) {
        throw RMDBError("Having clause without group by");
    }
}

Value Analyze::convert_sv_value(const std::shared_ptr<ast::Value>& sv_val) {
    Value val;
    if (auto int_lit = std::dynamic_pointer_cast<ast::IntLit>(sv_val)) {
        val.set_int(int_lit->val);
    } else if (auto float_lit =
                   std::dynamic_pointer_cast<ast::FloatLit>(sv_val)) {
        val.set_float(float_lit->val);
    } else if (auto str_lit =
                   std::dynamic_pointer_cast<ast::StringLit>(sv_val)) {
        val.set_str(str_lit->val);
    } else {
        throw InternalError("Unexpected sv value type");
    }
    return val;
}

CompOp Analyze::convert_sv_comp_op(ast::SvCompOp op) {
    std::map<ast::SvCompOp, CompOp> m = {
        {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
        {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
    };
    return m.at(op);
}
