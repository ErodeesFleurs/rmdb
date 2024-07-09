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
    //获取脏页
    std::unordered_map<page_id_t, RedoLogsInPage> dpt;
    auto tot_offset = 0;
    // std::cerr << "analyze start" << std::endl;
    // std::unordered_set<std::string> tables;
    // auto tot_offset = 0;
    // while (true) {
    //     buffer_.clear();
    //     int len = disk_manager_->read_log(buffer_.buffer_, LOG_BUFFER_SIZE,
    //                                       tot_offset);
    //     if (len <= 0) {
    //         break;
    //     }
    //     buffer_.offset_ += len;
    //     tot_offset += len;
    //     buffer_.cur_offset_ = 0;
    //     while (buffer_.has_next()) {
    //         auto log = buffer_.next();
    //         switch (log->log_type_) {
    //             case LogType::BEGIN:
    //             case LogType::COMMIT:
    //             case LogType::ABORT: {
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             case LogType::UPDATE: {
    //                 auto update_log =
    //                     std::dynamic_pointer_cast<UpdateLogRecord>(log);
    //                 tables.insert(update_log->table_name_);
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             case LogType::DELETE: {
    //                 auto delete_log =
    //                     std::dynamic_pointer_cast<DeleteLogRecord>(log);
    //                 tables.insert(delete_log->table_name_);
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             case LogType::INSERT: {
    //                 auto insert_log =
    //                     std::dynamic_pointer_cast<InsertLogRecord>(log);
    //                 tables.insert(insert_log->table_name_);
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             case LogType::INDEX_INSERT: {
    //                 auto insert_log =
    //                     std::dynamic_pointer_cast<IndexInsertLogRecord>(log);
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             case LogType::INDEX_DELETE: {
    //                 auto insert_log =
    //                     std::dynamic_pointer_cast<IndexDeleteLogRecord>(log);
    //                 att_[log->log_tid_] = log->lsn_;
    //                 break;
    //             }
    //             default:
    //                 break;
    //         }
    //         logs_.push_back(log);
    //     }
    // }
    // std::cerr << "analyze doing" << std::endl;
    // // 重建索引确保数据
    // for (const auto& tab_name : tables) {
    //     auto& tab = sm_manager_->db_.get_table(tab_name);
    //     for (const auto& index : tab.indexes) {
    //         auto index_name = sm_manager_->get_ix_manager()->get_index_name(
    //             tab.name, index.cols);
    //         auto index_manager = sm_manager_->get_ix_manager();
    //         std::cerr << "index_name: " << index_name << std::endl;
    //         // 如果当前索引被打开了, 先关闭
    //         if (sm_manager_->contains_index(index_name)) {
    //             index_manager->close_index(
    //                 sm_manager_->get_index_handle(index_name));
    //             sm_manager_->ihs_.erase(index_name);
    //         }
    //         std::cerr << "destroy index" << std::endl;
    //         index_manager->destroy_index(tab.name, index.cols);
    //         index_manager->create_index(tab.name, index.cols);
    //         sm_manager_->ihs_.emplace(
    //             index_name, index_manager->open_index(tab_name, index.cols));
    //     }
    // }
    // std::cerr << "analyze done" << std::endl;
    // std::cerr << "size: " << logs_.size() << std::endl;
}

/**
 * @description: 重做所有未落盘的操作
 */
void RecoveryManager::redo() {
    // rollback(true);
    // for (const auto& log_record : logs_) {
    //     if (auto log = std::dynamic_pointer_cast<BeginLogRecord>(log_record)) {
    //         continue;
    //     } else if (auto log =
    //                    std::dynamic_pointer_cast<CommitLogRecord>(log_record)) {
    //         continue;
    //     } else if (auto log =
    //                    std::dynamic_pointer_cast<AbortLogRecord>(log_record)) {
    //         continue;
    //     } else if (auto log =
    //                    std::dynamic_pointer_cast<InsertLogRecord>(log_record)) {
    //         auto file_handle = sm_manager_->get_file_handle(log->table_name_);
    //         try {
    //             // std::cerr << "redo: "
    //             //           << *reinterpret_cast<int*>(log->insert_value_.data)
    //             //           << std::endl;
    //             // std::cerr << "rid: " << log->rid_.page_no << ' '
    //             //           << log->rid_.slot_no << std::endl;
    //             file_handle->insert_record(log->rid_, log->insert_value_.data);
    //         } catch (RMDBError& e) {
    //             auto new_rid = file_handle->insert_record(
    //                 log->insert_value_.data, nullptr);
    //             log->rid_ = new_rid;
    //             std::cout << e.what() << '\n';
    //         }
    //     } else if (auto log =
    //                    std::dynamic_pointer_cast<DeleteLogRecord>(log_record)) {
    //         auto file_handle = sm_manager_->get_file_handle(log->table_name_);
    //         try {
    //             file_handle->delete_record(log->rid_, nullptr);
    //         } catch (RMDBError& e) {
    //             std::cout << e.what() << '\n';
    //         }
    //     } else if (auto log =
    //                    std::dynamic_pointer_cast<UpdateLogRecord>(log_record)) {
    //         auto file_handle = sm_manager_->get_file_handle(log->table_name_);
    //         try {
    //             file_handle->update_record(log->rid_, log->after_value_.data,
    //                                        nullptr);
    //         } catch (RMDBError& e) {
    //             std::cout << e.what() << '\n';
    //         }
    //     } else if (auto log = std::dynamic_pointer_cast<IndexInsertLogRecord>(
    //                    log_record)) {
    //         auto index_handle = sm_manager_->get_index_handle(log->ix_name_);
    //         index_handle->insert_entry(log->key_, log->rid_, nullptr);
    //     } else if (auto log = std::dynamic_pointer_cast<IndexDeleteLogRecord>(
    //                    log_record)) {
    //         auto index_handle = sm_manager_->get_index_handle(log->ix_name_);
    //         index_handle->delete_entry(log->key_, nullptr);
    //     }
    // }
}

/**
 * @description: 回滚未完成的事务
 */
void RecoveryManager::undo() {
    // rollback(false);
}

void RecoveryManager::rollback(bool is_r_txn) {
    for (auto it = att_.rbegin(); it != att_.rend(); ++it) {
        const auto [txn_id, lsn] = *it;
        auto idx = lsn;
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
            } else if (auto log =
                           std::dynamic_pointer_cast<IndexInsertLogRecord>(
                               logs_[idx])) {
                auto index_handle =
                    sm_manager_->get_index_handle(log->ix_name_);
                index_handle->delete_entry(log->key_, nullptr);
                idx = log->prev_lsn_;
            } else if (auto log =
                           std::dynamic_pointer_cast<IndexDeleteLogRecord>(
                               logs_[idx])) {
                auto index_handle =
                    sm_manager_->get_index_handle(log->ix_name_);
                index_handle->insert_entry(log->key_, log->rid_, nullptr);
                idx = log->prev_lsn_;
            }
        }
    }
}