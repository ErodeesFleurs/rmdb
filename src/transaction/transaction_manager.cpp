/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "transaction_manager.h"
#include "record/rm_file_handle.h"
#include "system/sm_manager.h"

std::unordered_map<txn_id_t, Transaction*> TransactionManager::txn_map = {};

/**
 * @description: 事务的开始方法
 * @return {Transaction*} 开始事务的指针
 * @param {Transaction*} txn 事务指针，空指针代表需要创建新事务，否则开始已有事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
Transaction* TransactionManager::begin(Transaction* txn,
                                       LogManager* log_manager) {
    // Todo:
    // 1. 判断传入事务参数是否为空指针
    // 2. 如果为空指针，创建新事务
    // 3. 把开始事务加入到全局事务表中
    // 4. 返回当前事务指针
    std::scoped_lock lock(latch_);
    if (txn == nullptr) {
        txn = new Transaction(next_txn_id_++);
    }
    txn_map.emplace(txn->get_transaction_id(), txn);
    //添加日志
    auto log = new BeginLogRecord(txn->get_transaction_id());
    log->prev_lsn_ = txn->get_prev_lsn();
    log_manager->add_log_to_buffer(log);
    txn->set_prev_lsn(log->lsn_);

    txn->set_state(TransactionState::DEFAULT);

    return txn;
}

/**
 * @description: 事务的提交方法
 * @param {Transaction*} txn 需要提交的事务
 * @param {LogManager*} log_manager 日志管理器指针
 */
void TransactionManager::commit(Transaction* txn, LogManager* log_manager) {
    // Todo:
    // 1. 如果存在未提交的写操作，提交所有的写操作
    // 2. 释放所有锁
    // 3. 释放事务相关资源，eg.锁集
    // 4. 把事务日志刷入磁盘中
    // 5. 更新事务状态

    auto lock_set = txn->get_lock_set();
    for (auto i : *lock_set) {
        lock_manager_->unlock(txn, i);
    }
    txn->clear();
    //添加日志
    auto log = new CommitLogRecord(txn->get_transaction_id());
    log->prev_lsn_ = txn->get_prev_lsn();
    log_manager->add_log_to_buffer(log);
    txn->set_prev_lsn(log->lsn_);

    txn->set_state(TransactionState::COMMITTED);
}

/**
 * @description: 事务的终止（回滚）方法
 * @param {Transaction *} txn 需要回滚的事务
 * @param {LogManager} *log_manager 日志管理器指针
 */
void TransactionManager::abort(Context* context, LogManager* log_manager) {
    // Todo:
    // 1. 回滚所有写操作
    // 2. 释放所有锁
    // 3. 清空事务相关资源，eg.锁集
    // 4. 把事务日志刷入磁盘中
    // 5. 更新事务状态

    auto txn = context->txn_;
    auto write_set = txn->get_write_set();
    while (!write_set->empty()) {
        auto write_record = write_set->back();
        write_set->pop_back();
        auto write_type = write_record->GetWriteType();
        auto table_name = write_record->GetTableName();
        auto record = write_record->GetRecord();
        auto rid = write_record->GetRid();
        if (!sm_manager_->contains_table(table_name)) {
            throw TableExistsError(table_name);
        }
        auto file_handle = sm_manager_->get_file_handle(table_name);
        // 皆反
        switch (write_type) {
            case WType::INSERT_TUPLE: {
                //更新日志
                auto log_record = std::make_unique<DeleteLogRecord>(
                    txn->get_transaction_id(), record, rid, table_name);
                log_record->prev_lsn_ = txn->get_prev_lsn();
                log_manager->add_log_to_buffer(log_record.get());
                txn->set_prev_lsn(log_record->lsn_);

                // context->lock_mgr_->lock_exclusive_on_record(
                //     txn, rid, file_handle->GetFd());
                delete_record_in_index(context, table_name, &record, rid);
                file_handle->delete_record(rid, context);
                break;
            }
            case WType::UPDATE_TUPLE: {
                auto old_record = file_handle->get_record(rid, context);
                //更新日志
                auto log_record =
                    new UpdateLogRecord(txn->get_transaction_id(), *old_record,
                                        record, rid, table_name);
                log_record->prev_lsn_ = txn->get_prev_lsn();
                log_manager->add_log_to_buffer(log_record);
                txn->set_prev_lsn(log_record->lsn_);

                // context->lock_mgr_->lock_exclusive_on_record(
                //     txn, rid, file_handle->GetFd());
                delete_record_in_index(context, table_name, old_record.get(),
                                       rid);
                file_handle->update_record(rid, record.data, context);
                insert_record_in_index(context, table_name, &record, rid);
                break;
            }
            case WType::DELETE_TUPLE: {
                //更新日志
                auto log_record = std::make_unique<InsertLogRecord>(
                    txn->get_transaction_id(), record, rid, table_name);
                log_record->prev_lsn_ = txn->get_prev_lsn();
                log_manager->add_log_to_buffer(log_record.get());
                txn->set_prev_lsn(log_record->lsn_);

                // context->lock_mgr_->lock_exclusive_on_table(
                //     txn, file_handle->GetFd());
                insert_record_in_index(context, table_name, &record, rid);
                file_handle->insert_record(rid, record.data);
                break;
            }
            default:
                throw InternalError("Unexpected write type");
        }
    }
    auto lock_set = txn->get_lock_set();
    for (auto i : *lock_set) {
        lock_manager_->unlock(txn, i);
    }
    txn->clear();

    auto log = std::make_unique<AbortLogRecord>(txn->get_transaction_id());
    log->prev_lsn_ = txn->get_prev_lsn();
    log_manager->add_log_to_buffer(log.get());
    txn->set_prev_lsn(log->lsn_);
    txn->set_state(TransactionState::ABORTED);
}

/**
 * @description: 删除索引中的记录
 * @param {const std::string&} table_name 表名
 * @param {RmRecord*} rec 记录指针
 * @param {Rid} rid_ 记录的Rid
 */
void TransactionManager::delete_record_in_index(Context* context,
                                                const std::string& table_name,
                                                RmRecord* rec, Rid rid_) {
    auto& tab = sm_manager_->db_.get_table(table_name);
    for (auto& index : tab.indexes) {
        auto index_name = sm_manager_->get_ix_manager()->get_index_name(
            table_name, index.cols);
        auto index_handle = sm_manager_->get_index_handle(index_name);
        auto key = std::make_unique<char[]>(index.col_tot_len);
        int offset = 0;
        for (int j = 0; j < index.col_num; ++j) {
            memcpy(key.get() + offset, rec->data + index.cols[j].offset,
                   index.cols[j].len);
            offset += index.cols[j].len;
        }

        //更新日志
        auto log = new IndexDeleteLogRecord(context->txn_->get_transaction_id(),
                                            key.get(), rid_, index_name,
                                            index.col_tot_len);
        log->prev_lsn_ = context->txn_->get_prev_lsn();
        context->log_mgr_->add_log_to_buffer(log);
        context->txn_->set_prev_lsn(log->lsn_);

        index_handle->delete_entry(key.get(), context->txn_);
    }
}

/**
 * @description: 在索引中插入记录
 * @param {const std::string&} table_name 表名
 * @param {RmRecord*} rec 记录指针
 * @param {Rid} rid_ 记录的Rid
 */
void TransactionManager::insert_record_in_index(Context* context,
                                                const std::string& table_name,
                                                RmRecord* rec, Rid rid_) {
    auto& tab = sm_manager_->db_.get_table(table_name);
    for (auto& index : tab.indexes) {
        auto index_name = sm_manager_->get_ix_manager()->get_index_name(
            table_name, index.cols);
        auto index_handle = sm_manager_->get_index_handle(index_name);
        auto key = std::make_unique<char[]>(index.col_tot_len);
        int offset = 0;
        for (int j = 0; j < index.col_num; ++j) {
            memcpy(key.get() + offset, rec->data + index.cols[j].offset,
                   index.cols[j].len);
            offset += index.cols[j].len;
        }
        //更新索引插入日志
        auto* index_log = new IndexInsertLogRecord(
            context->txn_->get_transaction_id(), key.get(), rid_, index_name,
            index.col_tot_len);
        index_log->prev_lsn_ = context->txn_->get_prev_lsn();
        context->log_mgr_->add_log_to_buffer(index_log);
        context->txn_->set_prev_lsn(index_log->lsn_);

        index_handle->insert_entry(key.get(), rid_, context->txn_);
    }
}