/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "log_recovery.h"

/**
 * @description: analyze阶段，需要获得脏页表（DPT）和未完成的事务列表（ATT）
 */
void RecoveryManager::analyze() {
    std::unordered_set<std::string> tables;
    while (buffer_.has_next()) {
        auto log = buffer_.next();
        switch (log->log_type_) {
            case LogType::BEGIN:
            case LogType::COMMIT:
            case LogType::ABORT: {
                att_[log->log_tid_] = log->lsn_;
                break;
            }
            case LogType::UPDATE: {
                auto update_log =
                    std::dynamic_pointer_cast<UpdateLogRecord>(log);
                tables.insert(update_log->table_name_);
                att_[log->log_tid_] = log->lsn_;
                break;
            }
            case LogType::DELETE: {
                auto delete_log =
                    std::dynamic_pointer_cast<DeleteLogRecord>(log);
                tables.insert(delete_log->table_name_);
                att_[log->log_tid_] = log->lsn_;
                break;
            }
            case LogType::INSERT: {
                auto insert_log =
                    std::dynamic_pointer_cast<InsertLogRecord>(log);
                tables.insert(insert_log->table_name_);
                att_[log->log_tid_] = log->lsn_;
                break;
            }
            default:
                break;
        }
        logs_.push_back(log);
    }
    // 重建索引确保数据
    for (const auto& tab_name : tables) {
        auto& tab = sm_manager_->db_.get_table(tab_name);
        for (const auto& index : tab.indexes) {
            auto index_name = sm_manager_->get_ix_manager()->get_index_name(
                tab.name, index.cols);
            auto index_manager = sm_manager_->get_ix_manager();
            // 如果当前索引被打开了
            if (sm_manager_->contains_index(index_name)) {
                index_manager->close_index(
                    sm_manager_->get_index_handle(index_name));
                sm_manager_->ihs_.erase(index_name);
            }
            index_manager->destroy_index(tab.name, index.cols);
            index_manager->create_index(tab.name, index.cols);
            sm_manager_->ihs_.emplace(
                index_name, index_manager->open_index(index_name, index.cols));
        }
    }
}

/**
 * @description: 重做所有未落盘的操作
 */
void RecoveryManager::redo() {
    rollback(true);
    for (const auto& log_reocrd : logs_) {
        if (auto log = std::dynamic_pointer_cast<BeginLogRecord>(log_reocrd)) {
            continue;
        } else if (auto log =
                       std::dynamic_pointer_cast<CommitLogRecord>(log_reocrd)) {
            continue;
        } else if (auto log =
                       std::dynamic_pointer_cast<AbortLogRecord>(log_reocrd)) {
            continue;
        } else if (auto log =
                       std::dynamic_pointer_cast<InsertLogRecord>(log_reocrd)) {
            auto file_handle = sm_manager_->get_file_handle(log->table_name_);
            try {
                file_handle->insert_record(log->rid_, log->insert_value_.data);
            } catch (RMDBError& e) {
                auto new_rid = file_handle->insert_record(
                    log->insert_value_.data, nullptr);
                assert(new_rid == log->rid_);
            }
        } else if (auto log =
                       std::dynamic_pointer_cast<DeleteLogRecord>(log_reocrd)) {
            auto file_handle = sm_manager_->get_file_handle(log->table_name_);
            try {
                file_handle->delete_record(log->rid_, nullptr);
            } catch (RMDBError& e) {
                std::cout << e.what() << '\n';
            }
        } else if (auto log =
                       std::dynamic_pointer_cast<UpdateLogRecord>(log_reocrd)) {
            auto file_handle = sm_manager_->get_file_handle(log->table_name_);
            try {
                file_handle->update_record(log->rid_, log->after_value_.data,
                                           nullptr);
            } catch (RMDBError& e) {
                std::cout << e.what() << '\n';
            }
        }
    }
}

/**
 * @description: 回滚未完成的事务
 */
void RecoveryManager::undo() {
    rollback(false);
}

void RecoveryManager::rollback(bool is_r_txn) {
    for (const auto [txn_id, lsn] : att_) {
        auto idx = txn_id;
        while (idx != INVALID_TXN_ID) {
            if (auto log =
                    std::dynamic_pointer_cast<BeginLogRecord>(logs_[idx])) {
                idx = log->prev_lsn_;
            } else if (auto log = std::dynamic_pointer_cast<CommitLogRecord>(
                           logs_[idx])) {
                if (is_r_txn) {
                    idx = log->prev_lsn_;
                } else {
                    break;
                }
            } else if (auto log = std::dynamic_pointer_cast<AbortLogRecord>(
                           logs_[idx])) {
                if (is_r_txn) {
                    idx = log->prev_lsn_;
                } else {
                    break;
                }
            } else if (auto log = std::dynamic_pointer_cast<InsertLogRecord>(
                           logs_[idx])) {
                auto file_handle =
                    sm_manager_->get_file_handle(log->table_name_);
                try {
                    file_handle->delete_record(log->rid_, nullptr);
                } catch (RMDBError& e) {
                    std::cout << e.what() << '\n';
                }
                idx = log->prev_lsn_;
            } else if (auto log = std::dynamic_pointer_cast<DeleteLogRecord>(
                           logs_[idx])) {
                auto file_handle =
                    sm_manager_->get_file_handle(log->table_name_);
                try {
                    file_handle->insert_record(log->rid_,
                                               log->delete_value_.data);
                } catch (RMDBError& e) {
                    std::cout << e.what() << '\n';
                }
                idx = log->prev_lsn_;
            } else if (auto log = std::dynamic_pointer_cast<UpdateLogRecord>(
                           logs_[idx])) {
                auto file_handle =
                    sm_manager_->get_file_handle(log->table_name_);
                try {
                    file_handle->update_record(
                        log->rid_, log->before_value_.data, nullptr);
                } catch (RMDBError& e) {
                    std::cout << e.what() << '\n';
                }
                idx = log->prev_lsn_;
            }
        }
    }
}