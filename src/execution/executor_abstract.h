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

#include "common/common.h"
#include "execution_defs.h"
#include "index/ix.h"
#include "system/sm.h"

enum class ExecutorType {
    UNKNOWN,
    ABSTRACT,
    SEQ_SCAN,
    INDEX_SCAN,
    GROUP,
    NESTED_LOOP_JOIN,
    PROJECTION,
    SORT,
    AGGREGATE,
    UPDATE,
    DELETE,
    INSERT,
    SELECT,
    CREATE,
    DROP
};

class AbstractExecutor {
   public:
    Rid _abstract_rid;

    Context* context_;

    virtual ~AbstractExecutor() = default;

    virtual size_t tupleLen() const { return 0; };

    virtual const std::vector<ColMeta>& cols() const {
        std::vector<ColMeta>* _cols = nullptr;
        return *_cols;
    };

    virtual ExecutorType getType() const { return ExecutorType::ABSTRACT; };

    virtual void beginTuple(){};

    virtual void nextTuple(){};

    virtual bool is_end() const { return true; };

    virtual Rid& rid() = 0;

    virtual std::unique_ptr<RmRecord> Next() = 0;

    virtual ColMeta get_col_offset(const TabCol& target) { return ColMeta(); };

    static std::vector<ColMeta>::const_iterator get_col(
        const std::vector<ColMeta>& rec_cols, const TabCol& target,
        bool cmp_table = true) {
        auto pos = std::find_if(
            rec_cols.begin(), rec_cols.end(), [&](const ColMeta& col) {
                return (!cmp_table || col.tab_name == target.tab_name) &&
                       col.name == target.col_name;
            });
        if (pos == rec_cols.end()) {
            throw ColumnNotFoundError(target.tab_name + '.' + target.col_name);
        }
        return pos;
    }

    static Value get_value(ColType p, const char* a) {
        Value res;
        switch (p) {
            case TYPE_INT: {
                int ia = *(int*)a;
                res.set_int(ia);
                break;
            }
            case TYPE_FLOAT: {
                double fa = *(double*)a;
                res.set_float(fa);
                break;
            }
            case TYPE_STRING:
                std::string str = a;
                res.set_str(str);
                break;
        }
        return res;
    }

    static void convert(Value& a, Value& b) {
        // 数值类型的转化(int, float)
        // int -> float
        if (a.type == b.type)
            return;
        if (a.type == TYPE_FLOAT) {
            if (b.type == TYPE_INT) {
                b.set_float((double)b.int_val);
                return;
            }
        } else if (a.type == TYPE_INT) {
            if (b.type == TYPE_FLOAT) {
                a.set_float((double)a.int_val);
                return;
            }
        }
        throw InternalError("convert::Unexpected value type");
    }

    static inline int val_compare(Value& pa, Value& pb) {
        convert(pa, pb);
        switch (pa.type) {
            case TYPE_FLOAT: {
                double va = pa.float_val;
                double vb = pb.float_val;
                return (va < vb) ? -1 : ((va > vb) ? 1 : 0);
            }
            case TYPE_INT: {
                int va = pa.int_val;
                int vb = pb.int_val;
                return (va < vb) ? -1 : ((va > vb) ? 1 : 0);
            }
            case TYPE_STRING: {
                auto same_size_str = pb.str_val;
                while (same_size_str.size() < pa.str_val.size()) {
                    same_size_str += char(0);
                }
                return (pa.str_val < same_size_str)
                           ? -1
                           : ((pa.str_val > same_size_str) ? 1 : 0);
            }
        }
        return 0;
    }

    static int comp_cond(const std::vector<ColMeta>& rec_cols,
                            const Condition& cond, const RmRecord* rec) {
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char* lhs = rec->data + lhs_col->offset;
        char* rhs;
        Value lhs_value = get_value(lhs_col->type, lhs);
        ColType rhs_type, lhs_type = lhs_col->type;
        assert(!cond.is_rhs_list && !cond.is_rhs_query && !cond.is_rhs_val);
        auto rhs_col = get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        int cmp;
        if (rhs_type != lhs_type) {
            Value ls = get_value(lhs_type, lhs);
            Value rs = get_value(rhs_type, rhs);
            cmp = val_compare(ls, rs);
        } else {
            cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        }
        assert(cond.op == OP_EQ);
        return cmp;
    }
    static bool eval_cond(const std::vector<ColMeta>& rec_cols,
                          const Condition& cond, const RmRecord* rec) {
        std::cerr << rec_cols[0].tab_name << " " << cond.lhs_col.tab_name << " <--------!!!!!" << std::endl;
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char* lhs = rec->data + lhs_col->offset;
        char* rhs;
        Value lhs_value = get_value(lhs_col->type, lhs);
        ColType rhs_type, lhs_type = lhs_col->type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else if (cond.is_rhs_query) {
            if (cond.rhs_query_res.first.size() != 1) {
                throw InternalError("sub_query::Unexpected colMetas size");
            }
            auto type = cond.rhs_query_res.first[0].type;
            if (type != lhs_type &&
                (type == TYPE_STRING || lhs_type == TYPE_STRING)) {
                throw InternalError("eval_cond::Unexpected type");
            }
            if (cond.rhs_query_res.second.size() == 0) {
                throw InternalError("eval_cond::Unexpected rhs_query_res size");
            }
            if (cond.rhs_query_res.second.size() == 1 && cond.op != OP_IN) {
                Value rhs_value =
                    get_value(type, cond.rhs_query_res.second[0].data +
                                        cond.rhs_query_res.first[0].offset);
                return check_cond(lhs_value, rhs_value, cond.op);
            }
            if (cond.op != OP_IN) {
                throw InternalError("eval_cond::Unexpected op type");
            }
            for (auto& record : cond.rhs_query_res.second) {
                Value rhs_value = get_value(
                    type, record.data + cond.rhs_query_res.first[0].offset);
                if (check_cond(lhs_value, rhs_value, OP_EQ)) {
                    return true;
                }
            }
            return false;
        } else if (cond.is_rhs_list) {
            if (cond.rhs_val_list.size() == 0) {
                throw InternalError("eval_cond::Unexpected rhs_val_list size");
            }
            if (cond.rhs_val_list.size() == 1 && cond.op != OP_IN) {
                if (lhs_type != cond.rhs_val_list[0].type &&
                    (lhs_type == TYPE_STRING || cond.rhs_val_list[0].type == TYPE_STRING)) {
                    return false;
                }
                return check_cond(lhs_value, cond.rhs_val_list[0], cond.op);
            }
            if (cond.op != OP_IN) {
                throw InternalError("eval_cond::Unexpected op type");
            }
            for (auto& value : cond.rhs_val_list) {
                if (lhs_type != value.type &&
                    (lhs_type == TYPE_STRING || value.type == TYPE_STRING)) {
                    continue;
                }
                if (check_cond(lhs_value, value, OP_EQ)) {
                    return true;
                }
            }
            return false;
        } else {
            // if (cond.rhs_col.tab_name != cond.lhs_col.tab_name) return true;
            auto rhs_col = get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        }
        int cmp;
        if (rhs_type != lhs_type) {
            Value ls = get_value(lhs_type, lhs);
            Value rs = get_value(rhs_type, rhs);
            cmp = val_compare(ls, rs);
        } else {
            cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        }
        if (cond.op == OP_EQ) {
            return cmp == 0;
        } else if (cond.op == OP_NE) {
            return cmp != 0;
        } else if (cond.op == OP_LT) {
            return cmp < 0;
        } else if (cond.op == OP_GT) {
            return cmp > 0;
        } else if (cond.op == OP_LE) {
            return cmp <= 0;
        } else if (cond.op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("eval_cond::Unexpected op type");
        }
    }

    static bool eval_index_cond(const std::vector<ColMeta>& rec_cols,
                          const Condition& cond, const RmRecord* rec) {
        std::cerr << rec_cols[0].tab_name << " " << cond.lhs_col.tab_name << " <--------!!!!!" << std::endl;
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char* lhs = rec->data + lhs_col->offset;
        char* rhs;
        Value lhs_value = get_value(lhs_col->type, lhs);
        ColType rhs_type, lhs_type = lhs_col->type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else if (cond.is_rhs_query) {
            if (cond.rhs_query_res.first.size() != 1) {
                throw InternalError("sub_query::Unexpected colMetas size");
            }
            auto type = cond.rhs_query_res.first[0].type;
            if (type != lhs_type &&
                (type == TYPE_STRING || lhs_type == TYPE_STRING)) {
                throw InternalError("eval_cond::Unexpected type");
            }
            if (cond.rhs_query_res.second.size() == 0) {
                throw InternalError("eval_cond::Unexpected rhs_query_res size");
            }
            if (cond.rhs_query_res.second.size() == 1 && cond.op != OP_IN) {
                Value rhs_value =
                    get_value(type, cond.rhs_query_res.second[0].data +
                                        cond.rhs_query_res.first[0].offset);
                return check_cond(lhs_value, rhs_value, cond.op);
            }
            if (cond.op != OP_IN) {
                throw InternalError("eval_cond::Unexpected op type");
            }
            for (auto& record : cond.rhs_query_res.second) {
                Value rhs_value = get_value(
                    type, record.data + cond.rhs_query_res.first[0].offset);
                if (check_cond(lhs_value, rhs_value, OP_EQ)) {
                    return true;
                }
            }
            return false;
        } else if (cond.is_rhs_list) {
            if (cond.rhs_val_list.size() == 0) {
                throw InternalError("eval_cond::Unexpected rhs_val_list size");
            }
            if (cond.rhs_val_list.size() == 1 && cond.op != OP_IN) {
                if (lhs_type != cond.rhs_val_list[0].type &&
                    (lhs_type == TYPE_STRING || cond.rhs_val_list[0].type == TYPE_STRING)) {
                    return false;
                }
                return check_cond(lhs_value, cond.rhs_val_list[0], cond.op);
            }
            if (cond.op != OP_IN) {
                throw InternalError("eval_cond::Unexpected op type");
            }
            for (auto& value : cond.rhs_val_list) {
                if (lhs_type != value.type &&
                    (lhs_type == TYPE_STRING || value.type == TYPE_STRING)) {
                    continue;
                }
                if (check_cond(lhs_value, value, OP_EQ)) {
                    return true;
                }
            }
            return false;
        } else {
            if (cond.rhs_col.tab_name != cond.lhs_col.tab_name) return true;
            auto rhs_col = get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        }
        int cmp;
        if (rhs_type != lhs_type) {
            Value ls = get_value(lhs_type, lhs);
            Value rs = get_value(rhs_type, rhs);
            cmp = val_compare(ls, rs);
        } else {
            cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        }
        if (cond.op == OP_EQ) {
            return cmp == 0;
        } else if (cond.op == OP_NE) {
            return cmp != 0;
        } else if (cond.op == OP_LT) {
            return cmp < 0;
        } else if (cond.op == OP_GT) {
            return cmp > 0;
        } else if (cond.op == OP_LE) {
            return cmp <= 0;
        } else if (cond.op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("eval_cond::Unexpected op type");
        }
    }

    static bool eval_conds(const std::vector<ColMeta>& rec_cols,
                           const std::vector<Condition>& conds,
                           const RmRecord* rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition& cond) {
                               return eval_cond(rec_cols, cond, rec);
                           });
    }

    static bool eval_index_conds(const std::vector<ColMeta>& rec_cols,
                           const std::vector<Condition>& conds,
                           const RmRecord* rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition& cond) {
                               return eval_index_cond(rec_cols, cond, rec);
                           });
    }

    static bool check_cond(Value left, Value right, CompOp op) {
        int cmp = val_compare(left, right);
        if (op == OP_EQ) {
            return cmp == 0;
        } else if (op == OP_NE) {
            return cmp != 0;
        } else if (op == OP_LT) {
            return cmp < 0;
        } else if (op == OP_GT) {
            return cmp > 0;
        } else if (op == OP_LE) {
            return cmp <= 0;
        } else if (op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("check_cond::Unexpected op type");
        }
    }

    static Value get_aggr_value(
        const std::vector<ColMeta>& rec_cols,
        const std::vector<std::unique_ptr<RmRecord>>& rec,
        const TabCol& tab_col, AggregateType agg_type) {
        Value val;
        ColMeta col_meta;
        if (agg_type == AggregateType::COUNT && tab_col.col_name == "*") {
            col_meta = ColMeta{.tab_name = "",
                               .name = "*",
                               .type = TYPE_INT,
                               .len = sizeof(int),
                               .offset = 0};
        } else {
            col_meta = *get_col(rec_cols, tab_col, false);
        }
        if (agg_type == AggregateType::NONE) {
            for (auto& col_meta : rec_cols) {
                if (col_meta.name == tab_col.col_name) {
                    if (col_meta.type == TYPE_INT) {
                        val.set_int(*(int*)(rec[0]->data + col_meta.offset));
                    } else if (col_meta.type == TYPE_FLOAT) {
                        val.set_float(
                            *(double*)(rec[0]->data + col_meta.offset));
                    } else {
                        val.set_str(std::string(rec[0]->data + col_meta.offset,
                                                col_meta.len));
                    }
                    break;
                }
            }
        } else if (agg_type == AggregateType::COUNT) {
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
                    max =
                        std::max(max, *(int*)(record->data + col_meta.offset));
                }
                val.set_int(max);
            } else if (col_meta.type == TYPE_FLOAT) {
                double max = std::numeric_limits<double>::min();
                for (const auto& record : rec) {
                    max = std::max(max,
                                   *(double*)(record->data + col_meta.offset));
                }
                val.set_float(max);
            } else if (col_meta.type == TYPE_STRING) {
                std::string max = "";
                for (const auto& record : rec) {
                    std::string str(record->data + col_meta.offset,
                                    col_meta.len);
                    max = std::max(max, str);
                }
                val.set_str(max);
            }
        } else if (agg_type == AggregateType::MIN) {
            if (col_meta.type == TYPE_INT) {
                int min = std::numeric_limits<int>::max();
                for (const auto& record : rec) {
                    min =
                        std::min(min, *(int*)(record->data + col_meta.offset));
                }
                val.set_int(min);
            } else if (col_meta.type == TYPE_FLOAT) {
                double min = std::numeric_limits<double>::max();
                for (const auto& record : rec) {
                    min = std::min(min,
                                   *(double*)(record->data + col_meta.offset));
                }
                val.set_float(min);
            } else if (col_meta.type == TYPE_STRING) {
                std::string min = std::string(255, 255);
                for (const auto& record : rec) {
                    std::string str(record->data + col_meta.offset,
                                    col_meta.len);
                    min = std::min(min, str);
                }
                val.set_str(min);
            }
        }
        return val;
    }
};