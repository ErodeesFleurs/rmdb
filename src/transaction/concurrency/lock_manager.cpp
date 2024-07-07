/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "lock_manager.h"

bool LockManager::CompareLockMode(LockMode mode1, LockMode mode2) {
    if (mode1 == LockMode::SHARED &&
        (mode2 == LockMode::SHARED || mode2 == LockMode::INTENTION_SHARED)) {
        return true;
    } else if (mode1 == LockMode::EXLUCSIVE) {
        return false;
    } else if (mode1 == LockMode::INTENTION_SHARED &&
               mode2 != LockMode::EXLUCSIVE) {
        return true;
    } else if (mode1 == LockMode::INTENTION_EXCLUSIVE &&
               (mode2 == LockMode::INTENTION_EXCLUSIVE ||
                mode2 == LockMode::INTENTION_SHARED)) {
        return true;
    } else if (mode1 == LockMode::S_IX && mode2 == LockMode::INTENTION_SHARED) {
        return true;
    }
    return false;
}

bool LockManager::CompareGroupLockWithLock(GroupLockMode group_mode,
                                           LockMode mode) {
    if (group_mode == GroupLockMode::NON_LOCK) {
        return true;
    } else
        return CompareLockMode(GetLockMode(group_mode), mode);
}

LockManager::GroupLockMode LockManager::GetGroupLockMode(LockMode mode) {
    switch (mode) {
        case LockMode::SHARED:
            return GroupLockMode::S;
        case LockMode::EXLUCSIVE:
            return GroupLockMode::X;
        case LockMode::INTENTION_SHARED:
            return GroupLockMode::IS;
        case LockMode::INTENTION_EXCLUSIVE:
            return GroupLockMode::IX;
        case LockMode::S_IX:
            return GroupLockMode::SIX;
        default:
            return GroupLockMode::NON_LOCK;
    }
}

LockManager::LockMode LockManager::GetLockMode(GroupLockMode mode) {
    switch (mode) {
        case GroupLockMode::S:
            return LockMode::SHARED;
        case GroupLockMode::X:
            return LockMode::EXLUCSIVE;
        case GroupLockMode::IS:
            return LockMode::INTENTION_SHARED;
        case GroupLockMode::IX:
            return LockMode::INTENTION_EXCLUSIVE;
        case GroupLockMode::SIX:
            return LockMode::S_IX;
        default:
            return LockMode::SHARED;
    }
}

/**
 * @description: 检查并获取普通锁，如果锁被其他事务占用，应用wait-die算法处理死锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {LockDataId&} lock_data_id 加锁的目标资源ID
 * @param {LockMode} lock_mode 加锁的模式
 */
bool LockManager::CheckAndGrantNormalLock(Transaction* txn,
                                          LockDataId& lock_data_id,
                                          LockMode lock_mode) {
    std::unique_lock<std::mutex> lock(latch_);

    txn->set_state(TransactionState::GROWING);

    auto exsit_lock = txn->get_lock_set()->find(lock_data_id);
    if (exsit_lock != txn->get_lock_set()->end()) {
        for (auto& lock_request : lock_table_[lock_data_id].request_queue_) {
            if (lock_request.txn_id_ == txn->get_transaction_id()) {
                if (lock_request.lock_mode_ == lock_mode) {
                    return true;
                } else if (lock_request.lock_mode_ == LockMode::EXLUCSIVE) {
                    return true;
                } else if (lock_data_id.type_ == LockDataType::TABLE &&
                           lock_request.lock_mode_ == LockMode::S_IX &&
                           lock_mode != LockMode::EXLUCSIVE) {
                    return true;
                } else {
                    break;
                }
            }
            if (lock_table_[lock_data_id].request_queue_.size() == 1) {
                // 如果当前队列中只有一个锁请求，则直接修改锁模式
                lock_table_[lock_data_id].request_queue_.front().lock_mode_ =
                    lock_mode;
                lock_table_[lock_data_id].group_lock_mode_ =
                    GetGroupLockMode(lock_mode);
            }
        }
    }

    auto& request_queue = lock_table_[lock_data_id].request_queue_;
    auto& group_lock_mode = lock_table_[lock_data_id].group_lock_mode_;
    if (request_queue.empty() && group_lock_mode == GroupLockMode::NON_LOCK) {
        // 如果队列为空，直接授予锁
        request_queue.emplace_back(txn->get_transaction_id(), lock_mode);
        request_queue.back().granted_ = true;
        txn->append_lock(lock_data_id);
        return true;
    }
    while (!CompareGroupLockWithLock(group_lock_mode, lock_mode)) {
        // 如果当前锁模式与队列中的锁模式冲突，根据wait-die算法处理死锁
        if (txn->get_transaction_id() > request_queue.front().txn_id_) {
            txn->set_state(TransactionState::ABORTED);
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        lock_table_[lock_data_id].cv_.wait(lock);
    }
    // 授予锁
    request_queue.emplace_back(txn->get_transaction_id(), lock_mode);
    request_queue.back().granted_ = true;
    txn->append_lock(lock_data_id);
    return true;
}

/**
 * @description: 直接获取意向锁，无需判断冲突
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {LockDataId&} lock_data_id 加锁的目标资源ID
 * @param {LockMode} lock_mode 加锁的模式
 */
bool LockManager::CheckAndGrantIntentLock(Transaction* txn,
                                          LockDataId& lock_data_id,
                                          LockMode lock_mode) {
    std::unique_lock<std::mutex> lock(latch_);
    auto& lock_request_queue = lock_table_[lock_data_id];

    // 添加意向锁请求到队列并授予锁
    lock_request_queue.request_queue_.emplace_back(txn->get_transaction_id(),
                                                   lock_mode);
    lock_request_queue.request_queue_.back().granted_ = true;
    txn->append_lock(lock_data_id);
    // std::cerr << "lock success" << std::endl;
    return true;
}

/**
 * @description: 申请行级共享锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID 记录所在的表的fd
 * @param {int} tab_fd
 */
bool LockManager::lock_shared_on_record(Transaction* txn, const Rid& rid,
                                        int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock shared on record"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    return CheckAndGrantNormalLock(txn, lock_data_id, LockMode::SHARED);
}

/**
 * @description: 申请行级排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_record(Transaction* txn, const Rid& rid,
                                           int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock exclusive on record"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    return CheckAndGrantNormalLock(txn, lock_data_id, LockMode::EXLUCSIVE);
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock shared on table"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantNormalLock(txn, lock_data_id, LockMode::SHARED);
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock exclusive on table"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantNormalLock(txn, lock_data_id, LockMode::EXLUCSIVE);
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock IS on table" << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantNormalLock(txn, lock_data_id,
                                   LockMode::INTENTION_SHARED);
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock IX on table" << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantNormalLock(txn, lock_data_id,
                                   LockMode::INTENTION_EXCLUSIVE);
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    // std::cerr << txn->get_transaction_id() << " unlock" << " "
    //           << lock_data_id.type_ << std::endl;
    std::unique_lock<std::mutex> lock(latch_);

    txn->set_state(TransactionState::SHRINKING);
    auto& request_queue = lock_table_[lock_data_id].request_queue_;
    auto it = std::find_if(request_queue.begin(), request_queue.end(),
                           [&txn](const LockRequest& lock_request) {
                               return lock_request.txn_id_ ==
                                      txn->get_transaction_id();
                           });
    if (it != request_queue.end()) {
        request_queue.erase(it);
        if (request_queue.empty()) {
            lock_table_[lock_data_id].group_lock_mode_ =
                GroupLockMode::NON_LOCK;
        } else {
            lock_table_[lock_data_id].group_lock_mode_ =
                GetGroupLockMode(request_queue.front().lock_mode_);
        }
    } else {
        return true;
    }
    lock_table_[lock_data_id].cv_.notify_one();
    return true;
}