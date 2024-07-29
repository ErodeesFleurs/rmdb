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
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "defs.h"
#include "parser/parser.h"
#include "record/rm_defs.h"
#include "system/sm_meta.h"

using SelectResult = std::pair<std::vector<ColMeta>, std::vector<RmRecord>>;

struct TabCol {
    std::string tab_name;
    std::string col_name;

    std::string as_name;
    AggregateType aggregate;

    friend bool operator<(const TabCol& x, const TabCol& y) {
        return std::make_pair(x.tab_name, x.col_name) <
               std::make_pair(y.tab_name, y.col_name);
    }

    friend bool operator==(const TabCol& x, const TabCol& y) {
        return std::make_pair(x.tab_name, x.col_name) ==
               std::make_pair(y.tab_name, y.col_name);
    }
};

struct Value {
    ColType type;  // type of value

    std::variant<int, double, std::string> val;

    std::shared_ptr<RmRecord> raw;  // raw record buffer

    template <typename T>
    Value& operator=(T&& val) {
        if constexpr (std::is_same_v<T, int>) {
            set_int(val);
        } else if constexpr (std::is_same_v<T, double>) {
            set_float(val);
        } else if constexpr (std::is_same_v<T, std::string>) {
            set_str(val);
        } else {
            throw std::runtime_error("Invalid value type");
        }
        return *this;
    }

    void set_int(int int_val_) {
        type = TYPE_INT;
        val = int_val_;
    }

    void set_float(double float_val_) {
        type = TYPE_FLOAT;
        val = float_val_;
    }

    void set_str(std::string str_val_) {
        type = TYPE_STRING;
        val = std::move(str_val_);
    }

    bool to_floor() {
        if (type == TYPE_FLOAT) {
            val = std::floor(std::get<double>(val));
            return true;
        }
        return false;
    }

    bool to_cell() {
        if (type == TYPE_FLOAT) {
            val = std::ceil(std::get<double>(val));
            return true;
        }
        return false;
    }

    bool to_int() {
        if (type == TYPE_FLOAT) {
            val = (int)std::get<double>(val);
            type = TYPE_INT;
            return true;
        }
        return false;
    }

    bool to_float() {
        if (type == TYPE_INT) {
            val = (double)std::get<int>(val);
            type = TYPE_FLOAT;
            return true;
        }
        return false;
    }

    void init_raw(int len) {
        assert(raw == nullptr);
        raw = std::make_shared<RmRecord>(len);
        if (type == TYPE_INT) {
            assert(len == sizeof(int));
            *(int*)(raw->data) = std::get<int>(val);
        } else if (type == TYPE_FLOAT) {
            assert(len == sizeof(double));
            *(double*)(raw->data) = std::get<double>(val);
        } else if (type == TYPE_STRING) {
            auto str = std::get<std::string>(val);
            if (len < (int)str.size()) {
                throw StringOverflowError();
            }
            memset(raw->data, 0, len);
            memcpy(raw->data, str.c_str(), str.size());
        }
    }

    void init_raw() {
        assert(raw == nullptr);
        if (type == TYPE_INT) {
            raw = std::make_shared<RmRecord>(sizeof(int));
            *(int*)(raw->data) = std::get<int>(val);
        } else if (type == TYPE_FLOAT) {
            raw = std::make_shared<RmRecord>(sizeof(double));
            *(double*)(raw->data) = std::get<double>(val);
        } else if (type == TYPE_STRING) {
            auto str_val = std::get<std::string>(val);
            raw = std::make_shared<RmRecord>(str_val.size());
            memcpy(raw->data, str_val.c_str(), str_val.size());
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Value& val) {
        switch (val.type) {
            case TYPE_INT:
                os << std::get<int>(val.val);
                break;
            case TYPE_FLOAT:
                os << std::get<double>(val.val);
                break;
            case TYPE_STRING:
                os << std::get<std::string>(val.val);
                break;
            default:
                throw std::runtime_error("Invalid value type");
        }
        return os;
    }

    friend bool operator==(const Value& x, const Value& y) {
        if (x.type != y.type)
            return false;
        switch (x.type) {
            case TYPE_INT:
                return std::get<int>(x.val) == std::get<int>(y.val);
            case TYPE_FLOAT:
                return std::get<double>(x.val) == std::get<double>(y.val);
            case TYPE_STRING:
                return std::get<std::string>(x.val) ==
                       std::get<std::string>(y.val);
            default:
                return false;
        }
    }

    friend bool operator!=(const Value& x, const Value& y) { return !(x == y); }

    friend bool operator<(const Value& x, const Value& y) {
        if (x.type != y.type)
            return x.type < y.type;
        switch (x.type) {
            case TYPE_INT:
                return std::get<int>(x.val) < std::get<int>(y.val);
            case TYPE_FLOAT:
                return std::get<double>(x.val) < std::get<double>(y.val);
            case TYPE_STRING:
                return std::get<std::string>(x.val) <
                       std::get<std::string>(y.val);
            default:
                return false;
        }
    }

    friend bool operator>(const Value& x, const Value& y) { return y < x; }

    friend bool operator<=(const Value& x, const Value& y) { return !(y < x); }

    friend bool operator>=(const Value& x, const Value& y) { return !(x < y); }

    friend Value operator+(const Value& x, const Value& y) {
        Value res;
        if (x.type == TYPE_INT && y.type == TYPE_INT) {
            res.set_int(std::get<int>(x.val) + std::get<int>(y.val));
        } else if (x.type == TYPE_FLOAT && y.type == TYPE_FLOAT) {
            res.set_float(std::get<double>(x.val) + std::get<double>(y.val));
        } else {
            throw std::runtime_error("Invalid operation");
        }
        return res;
    }
};

enum CompOp { OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE, OP_IN };

enum SetOp { OP_ADD, OP_SUB, OP_SET };

struct Query;
struct Condition {
    TabCol lhs_col;     // left-hand side column
    CompOp op;          // comparison operator
    bool is_rhs_val;    // true if right-hand side is a value (not a column)
    bool is_rhs_query;  // true if right-hand side is a query
    bool is_rhs_list;   // true if right-hand side is a value list
    TabCol rhs_col;     // right-hand side column
    Value rhs_val;      // right-hand side value
    SelectResult rhs_query_res;       // right-hand side query
    std::vector<Value> rhs_val_list;  // right-hand side value list
};

struct SetClause {
    TabCol lhs;
    Value rhs;
    SetOp op;
};

class Query {
   public:
    std::shared_ptr<ast::TreeNode> parse;
    // TODO jointree
    // where条件
    std::vector<Condition> conds;
    // 投影列
    std::vector<TabCol> cols;
    // 表名
    std::vector<std::string> tables;
    // update 的set 值
    std::vector<SetClause> set_clauses;
    //insert 的values值
    std::vector<Value> values;

    std::vector<TabCol> group_cols;

    std::vector<Condition> having_conds;

    Query() {}
};
