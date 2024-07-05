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

/**
 * @description: 申请行级共享锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID 记录所在的表的fd
 * @param {int} tab_fd
 */
bool LockManager::lock_shared_on_record(Transaction* txn, const Rid& rid,
                                        int tab_fd) {
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }

    // if (!WaitDie(request_queue, request)) {
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
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
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }

    // if (!WaitDie(request_queue, request)) {
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }

    // if (!WaitDie(request_queue, request)) {
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }

    // if (!WaitDie(request_queue, request)) {
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
}
/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(), LockMode::INTENTION_SHARED);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }

    // if (!WaitDie(request_queue, request)) {
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    // std::unique_lock<std::mutex> lock(latch_);
    // LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    // auto& request_queue = lock_table_[lock_data_id];
    // LockRequest request(txn->get_transaction_id(),
    //                     LockMode::INTENTION_EXCLUSIVE);
    // request_queue.request_queue_.push_back(request);

    // if (txn->get_state() == TransactionState::DEFAULT) {
    //     txn->set_state(TransactionState::GROWING);
    // }
    // std::cerr << "lock_IX_on_table: wait die: "
    //           << WaitDie(request_queue, request) << std::endl;
    // if (!WaitDie(request_queue, request)) {
    //     std::cerr << "lock_IX_on_table: grant lock" << std::endl;
    //     while (!GrantLock(request_queue, request)) {
    //         request_queue.cv_.wait(lock);
    //     }
    //     txn->append_lock_set(lock_data_id);
    // } else {
    //     throw TransactionAbortException(txn->get_transaction_id(),
    //                                     AbortReason::DEADLOCK_PREVENTION);
    // }

    return true;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    // std::unique_lock<std::mutex> lock(latch_);
    // auto it = lock_table_.find(lock_data_id);
    // if (it == lock_table_.end()) {
    //     return false;
    // }

    // auto& request_queue = it->second;
    // auto request_it = std::find_if(
    //     request_queue.request_queue_.begin(),
    //     request_queue.request_queue_.end(), [&](const LockRequest& req) {
    //         return req.txn_id_ == txn->get_transaction_id();
    //     });

    // if (request_it == request_queue.request_queue_.end()) {
    //     return false;
    // }

    // request_queue.request_queue_.erase(request_it);
    // request_queue.group_lock_mode_ = CalculateGroupLockMode(request_queue);
    // request_queue.cv_.notify_all();

    // if (request_queue.request_queue_.empty()) {
    //     lock_table_.erase(it);
    // }

    // // 更新事务状态为 SHRINKING
    // txn->set_state(TransactionState::SHRINKING);
    return true;
}

/**
 * @description: 判断事务是否加锁
 * @return {bool} 返回是否加锁
 * @param {Transaction*} txn 要判断的事务对象指针
 */
bool LockManager::is_lock(Transaction* txn) {
    std::unique_lock<std::mutex> lock(latch_);
    for (const auto& entry : lock_table_) {
        for (const auto& request : entry.second.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id() &&
                request.granted_) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @description: 判断事务是否解锁
 * @return {bool} 返回是否解锁
 * @param {Transaction*} txn 要判断的事务对象指针
 */
bool LockManager::is_unlock(Transaction* txn) {
    return !is_lock(txn);
}
/**
 * @description: 授予锁
 * @return {bool} 返回是否授予锁
 * @param {LockRequestQueue&} request_queue
 * @param {LockRequest&} request
 */
bool LockManager::GrantLock(LockRequestQueue& request_queue,
                            LockRequest& request) {
    std::cerr << "GrantLock: " << request_queue.group_lock_mode_ << std::endl;
    if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK ||
        request_queue.group_lock_mode_ == GroupLockMode::IS) {
        request.granted_ = true;
        request_queue.group_lock_mode_ =
            (request.lock_mode_ == LockMode::SHARED) ? GroupLockMode::S
                                                     : GroupLockMode::X;
        return true;
    }
    return false;
}

bool LockManager::CanGrantLock(const LockRequestQueue& request_queue,
                               LockMode mode) {
    switch (request_queue.group_lock_mode_) {
        case GroupLockMode::NON_LOCK:
            return true;
        case GroupLockMode::IS:
            return mode == LockMode::SHARED ||
                   mode == LockMode::INTENTION_SHARED;
        case GroupLockMode::IX:
            return mode == LockMode::INTENTION_SHARED ||
                   mode == LockMode::INTENTION_EXCLUSIVE;
        case GroupLockMode::S:
            return mode == LockMode::SHARED;
        case GroupLockMode::X:
        case GroupLockMode::SIX:
            return false;
        default:
            return false;
    }
}

LockManager::GroupLockMode LockManager::CalculateGroupLockMode(
    const LockRequestQueue& request_queue) {
    GroupLockMode mode = GroupLockMode::NON_LOCK;
    for (const auto& req : request_queue.request_queue_) {
        if (req.granted_) {
            switch (req.lock_mode_) {
                case LockMode::SHARED:
                    mode = GroupLockMode::S;
                    break;
                case LockMode::EXLUCSIVE:
                    mode = GroupLockMode::X;
                    break;
                case LockMode::INTENTION_SHARED:
                    mode = GroupLockMode::IS;
                    break;
                case LockMode::INTENTION_EXCLUSIVE:
                    mode = GroupLockMode::IX;
                    break;
                case LockMode::S_IX:
                    mode = GroupLockMode::SIX;
                    break;
            }
        }
    }
    return mode;
}
/** 
 * @description: 等待图死锁检测, 如果当前请求的事务ID小于已经获得锁的事务ID，则返回true，否则返回false
 * @param {LockRequestQueue&} request_queue
 * @param {LockRequest&} request
 * @return {bool}
 */
bool LockManager::WaitDie(LockRequestQueue& request_queue,
                          LockRequest& request) {
    std::cerr << "WaitDie: " << request.txn_id_ << std::endl;
    for (const auto& req : request_queue.request_queue_) {
        std::cerr << "WaitDie REQ: " << req.txn_id_ << std::endl;
        if (req.granted_ && req.txn_id_ < request.txn_id_) {
            return true;
        } else if (req.granted_ && req.txn_id_ > request.txn_id_) {
            throw TransactionAbortException(request.txn_id_,
                                            AbortReason::DEADLOCK_PREVENTION);
        }
    }
    return false;
}