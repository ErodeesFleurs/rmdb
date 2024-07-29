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
    RmFileHandle* fh_;
    std::vector<Rid> rids_;
    std::string tab_name_;
    std::vector<SetClause> set_clauses_;
    SmManager* sm_manager_;

   public:
    UpdateExecutor(SmManager* sm_manager, const std::string& tab_name,
                   std::vector<SetClause> set_clauses,
                   std::vector<Condition> conds, std::vector<Rid> rids,
                   Context* context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        set_clauses_ = set_clauses;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;

        // if (context_->txn_ != nullptr) {
        //     context_->lock_mgr_->lock_IX_on_table(
        //         context->txn_, sm_manager_->fhs_[tab_name_]->GetFd());
        // }
    }

    void delete_index(RmRecord* rec, Rid rid_) {
        // 删除索引
        for (auto& index : tab_.indexes) {
            auto ix_name = sm_manager_->get_ix_manager()->get_index_name(
                tab_name_, index.cols);
            auto ih = sm_manager_->ihs_.at(ix_name).get();
            auto key = std::make_unique<char[]>(index.col_tot_len);
            int offset = 0;
            for (int j = 0; j < index.col_num; ++j) {
                memcpy(key.get() + offset, rec->data + index.cols[j].offset,
                       index.cols[j].len);
                offset += index.cols[j].len;
            }
            //更新日志
            auto logRecord = new IndexDeleteLogRecord(
                context_->txn_->get_transaction_id(), key.get(), rid_, ix_name,
                index.col_tot_len);
            logRecord->prev_lsn_ = context_->txn_->get_prev_lsn();
            context_->log_mgr_->add_log_to_buffer(logRecord);
            context_->txn_->set_prev_lsn(logRecord->lsn_);
            //删除索引
            ih->delete_entry(key.get(), context_->txn_);
        }
    }

    bool insert_index(RmRecord* rec, Rid rid_) {
        // 插入索引
        int fail_p = -1;
        for (int i = 0; i < (int)tab_.indexes.size(); i++) {
            auto& index = tab_.indexes[i];
            auto ix_name = sm_manager_->get_ix_manager()->get_index_name(
                tab_name_, index.cols);
            auto ih = sm_manager_->ihs_.at(ix_name).get();
            auto key = std::make_unique<char[]>(index.col_tot_len);
            int offset = 0;
            for (int j = 0; j < index.col_num; ++j) {
                memcpy(key.get() + offset, rec->data + index.cols[j].offset,
                       index.cols[j].len);
                offset += index.cols[j].len;
            }
            //更新日志
            auto logRecord = new IndexInsertLogRecord(
                context_->txn_->get_transaction_id(), key.get(), rid_, ix_name,
                index.col_tot_len);
            logRecord->prev_lsn_ = context_->txn_->get_prev_lsn();
            context_->log_mgr_->add_log_to_buffer(logRecord);
            context_->txn_->set_prev_lsn(logRecord->lsn_);
            //插入索引
            auto result = ih->insert_entry(key.get(), rid_, context_->txn_);
            if (result.second == false) {
                fail_p = i;
                break;
            }
        }
        if (fail_p != -1) {
            //说明插入失败，需要rollback
            //删掉已插入索引
            for (int i = 0; i < fail_p; i++) {
                auto& index = tab_.indexes[i];
                auto ix_name = sm_manager_->get_ix_manager()->get_index_name(
                    tab_name_, index.cols);
                auto ih = sm_manager_->ihs_.at(ix_name).get();
                auto key = std::make_unique<char[]>(index.col_tot_len);
                int offset = 0;
                for (int j = 0; j < index.col_num; ++j) {
                    memcpy(key.get() + offset, rec->data + index.cols[j].offset,
                           index.cols[j].len);
                    offset += index.cols[j].len;
                }
                //更新日志
                auto* logRecord = new IndexDeleteLogRecord(
                    context_->txn_->get_transaction_id(), key.get(), rid_,
                    ix_name, index.col_tot_len);
                logRecord->prev_lsn_ = context_->txn_->get_prev_lsn();
                context_->log_mgr_->add_log_to_buffer(logRecord);
                context_->txn_->set_prev_lsn(logRecord->lsn_);
                //删除索引
                ih->delete_entry(key.get(), context_->txn_);
            }
            return false;
        }
        return true;
    }

    std::unique_ptr<RmRecord> Next() override {
        std::map<TabCol, ColMeta> mp;
        for (const auto& i : set_clauses_) {
            ColMeta col = *get_col(tab_.cols, i.lhs);
            mp[i.lhs] = col;
        }
        bool is_fail = false;
        int upd_cnt = 0;
        for (auto rid : rids_) {
            auto rec = fh_->get_record(rid, context_);
            auto old_rec = fh_->get_record(rid, context_);
            // if (context_->txn_ != nullptr)
            //     context_->lock_mgr_->lock_exclusive_on_record(
            //         context_->txn_, rid, fh_->GetFd());
            delete_index(rec.get(), rid);
            upd_cnt++;
            for (const auto& i : set_clauses_) {
                auto col = mp[i.lhs];
                auto value = i.rhs;
                if (value.type != col.type) {
                    Value b = {.type = col.type};
                    convert(value, b);
                    if (value.type != col.type) {
                        throw IncompatibleTypeError(coltype2str(col.type),
                                                    coltype2str(value.type));
                    }
                }
                char* rec_buf = rec->data + col.offset;
                if (col.type == TYPE_INT) {
                    auto old_val = *(int*)rec_buf;
                    if (i.op == SetOp::OP_ADD) {
                        std::get<int>(value.val) += old_val;
                    } else if (i.op == SetOp::OP_SUB) {
                        std::get<int>(value.val) -= old_val;
                    }
                } else if (col.type == TYPE_FLOAT) {
                    auto old_val = *(double*)rec_buf;
                    if (i.op == SetOp::OP_ADD) {
                        std::get<double>(value.val) += old_val;
                    } else if (i.op == SetOp::OP_SUB) {
                        std::get<double>(value.val) -= old_val;
                    }
                } else if (col.type == TYPE_STRING) {
                    //do nothing
                }
                value.init_raw(col.len);
                //更新记录数据
                memcpy(rec->data + col.offset, value.raw->data, col.len);
            }
            if (!insert_index(rec.get(), rid)) {
                is_fail = true;
                insert_index(old_rec.get(), rid);
                upd_cnt--;
                break;
            }
            //更新日志
            auto logRecord =
                new UpdateLogRecord(context_->txn_->get_transaction_id(),
                                    *old_rec, *rec, rid, tab_name_);
            logRecord->prev_lsn_ = context_->txn_->get_prev_lsn();
            context_->log_mgr_->add_log_to_buffer(logRecord);
            context_->txn_->set_prev_lsn(logRecord->lsn_);
            //更新记录
            fh_->update_record(rid, rec->data, context_);
            //更新事务
            auto* wr =
                new WriteRecord(WType::UPDATE_TUPLE, tab_name_, rid, *old_rec);
            context_->txn_->append_write_record(wr);
        }

        if (is_fail) {
            //插入失败
            while (upd_cnt--) {
                auto last = context_->txn_->get_last_write_record();
                auto type = last->GetWriteType();
                assert(type == WType::UPDATE_TUPLE);
                auto rid_ = last->GetRid();
                auto tab_name = last->GetTableName();
                auto rec_ = last->GetRecord();
                auto now_rec = fh_->get_record(rid_, context_);
                delete_index(now_rec.get(), rid_);
                insert_index(&rec_, rid_);
                //更新日志
                auto logRecord =
                    new UpdateLogRecord(context_->txn_->get_transaction_id(),
                                        *now_rec, rec_, rid_, tab_name_);
                logRecord->prev_lsn_ = context_->txn_->get_prev_lsn();
                context_->log_mgr_->add_log_to_buffer(logRecord);
                context_->txn_->set_prev_lsn(logRecord->lsn_);
                //更新记录
                fh_->update_record(rid_, rec_.data, context_);
                context_->txn_->delete_write_record();
            }
            throw RMDBError("Update Error");
        }
        return nullptr;
    }

    Rid& rid() override { return _abstract_rid; }

    ExecutorType getType() const override { return ExecutorType::UPDATE; }
};