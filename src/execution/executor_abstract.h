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
#include "common/common.h"
#include "index/ix.h"
#include "system/sm.h"

class AbstractExecutor {
   public:
    Rid _abstract_rid;

    Context *context_;

    virtual ~AbstractExecutor() = default;

    virtual size_t tupleLen() const { return 0; };

    virtual const std::vector<ColMeta> &cols() const {
        std::vector<ColMeta> *_cols = nullptr;
        return *_cols;
    };

    virtual std::string getType() { return "AbstractExecutor"; };

    virtual void beginTuple(){};

    virtual void nextTuple(){};

    virtual bool is_end() const { return true; };

    virtual Rid &rid() = 0;

    virtual std::unique_ptr<RmRecord> Next() = 0;

    virtual ColMeta get_col_offset(const TabCol &target) { return ColMeta();};

    static std::vector<ColMeta>::const_iterator get_col(const std::vector<ColMeta> &rec_cols, const TabCol &target) {
        auto pos = std::find_if(rec_cols.begin(), rec_cols.end(), [&](const ColMeta &col) {
            return col.tab_name == target.tab_name && col.name == target.col_name;
        });
        if (pos == rec_cols.end()) {
            throw ColumnNotFoundError(target.tab_name + '.' + target.col_name);
        }
        return pos;
    }

    static Value get_value(ColType p, const char *a) {
        Value res;
        switch (p) {
            case TYPE_INT: {
                int ia = *(int *) a;
                res.set_int(ia);
                break;
            }
            case TYPE_FLOAT: {
                double fa = *(double *) a;
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

    static void convert(Value &a, Value &b) {
        // 数值类型的转化(int, float)
        // int -> float
        if (a.type == b.type) return;
        if (a.type == TYPE_FLOAT) {
            if (b.type == TYPE_INT) {
                b.set_float((double) b.int_val);
                return;
            }
        }
        else if (a.type == TYPE_INT) {
            if (b.type == TYPE_FLOAT) {
                a.set_float((double) a.int_val);
                return;
            }
        }
        throw InternalError("convert::Unexpected value type");
    }

    static inline int val_compare(Value &pa, Value &pb) {
        convert(pa, pb);
        switch (pa.type) {
            case TYPE_FLOAT:{
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
                return pa.str_val.compare(pb.str_val);
            }
        }
        return 0;
    }

    static bool eval_cond(const std::vector<ColMeta> &rec_cols, const Condition &cond, const RmRecord *rec) {
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char *lhs = rec->data + lhs_col->offset;
        char *rhs;
        ColType rhs_type, lhs_type = lhs_col->type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else {
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

    bool eval_conds(const std::vector<ColMeta> &rec_cols, const std::vector<Condition> &conds, const RmRecord *rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition &cond) { return eval_cond(rec_cols, cond, rec); });
    }

    bool check_cond(Value left, Value right, CompOp op) {
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


};