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
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
    auto& request_queue = lock_table_[lock_data_id];
    bool is_ok = true;
    for (const auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_) {
                return true;
            }
            is_ok = false;
            break;
        }
    }
    if (is_ok) {
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::SHARED};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;

        // 表上有X锁不能申请
        LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id_table];
        for (const auto& request : lock_request_queue.request_queue_) {
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = true;
                break;
            }
        }
        // 行上有X锁不能申请
        LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
        auto& request_queue = lock_table_[lock_data_id];
        for (const auto& request : request_queue.request_queue_) {
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = true;
                break;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        // 行上加S锁
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::SHARED};
        if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK) {
            request_queue.group_lock_mode_ = GroupLockMode::S;
        }
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
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
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
    auto& request_queue = lock_table_[lock_data_id];
    bool is_ok = true;
    for (auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_ && request.lock_mode_ == LockMode::EXLUCSIVE) {
                return true;
            }
            request.granted_ = false;
            is_ok = false;
            break;
        }
    }
    if (is_ok) {
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;
        LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id_table];
        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = true;
                break;
            }
            // 表上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = true;
                break;
            }
            // 表上有SIX锁不能申请
            if (request.lock_mode_ == LockMode::S_IX && request.granted_) {
                flag = true;
                break;
            }
        }

        LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
        auto& request_queue = lock_table_[lock_data_id];
        for (const auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 行上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = true;
                break;
            }
            // 行上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = true;
                break;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        // 行上加X锁
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue.group_lock_mode_ = GroupLockMode::X;
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.lock_mode_ = LockMode::EXLUCSIVE;
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
    auto& request_queue = lock_table_[lock_data_id_table];
    bool is_ok = true;
    for (const auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_)
                return true;
            is_ok = false;
            break;
        }
    }
    if (is_ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::SHARED};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_table);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;
        LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id_table];

        for (const auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 表上有IX锁不能申请
            if (request.lock_mode_ == LockMode::INTENTION_EXCLUSIVE &&
                request.granted_) {
                flag = true;
                break;
            }
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = true;
                break;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        // 表上加S锁
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& request_queue = lock_table_[lock_data_id];
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::SHARED};
        if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK)
            request_queue.group_lock_mode_ = GroupLockMode::S;
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    auto& request_queue = lock_table_[lock_data_id];
    bool is_ok = true;
    for (auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_ && request.lock_mode_ == LockMode::EXLUCSIVE) {
                return true;
            }
            is_ok = false;
            break;
        }
    }
    if (is_ok) {
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& request_queue = lock_table_[lock_data_id];
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ != txn->get_transaction_id()) {
                //有人持有锁
                if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                    request.granted_)
                    flag = true;
                if (request.lock_mode_ == LockMode::INTENTION_EXCLUSIVE &&
                    request.granted_)
                    flag = true;
                if (request.lock_mode_ == LockMode::SHARED && request.granted_)
                    flag = true;
                if (request.lock_mode_ == LockMode::INTENTION_SHARED &&
                    request.granted_)
                    flag = true;
                if (request.lock_mode_ == LockMode::S_IX && request.granted_)
                    flag = true;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        // 表上加X锁
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue.group_lock_mode_ = GroupLockMode::X;
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.lock_mode_ = LockMode::EXLUCSIVE;
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
    auto& request_queue = lock_table_[lock_data_id_table];
    bool is_ok = true;
    for (const auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_) {
                return true;
            }
            is_ok = false;
            break;
        }
    }
    if (is_ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_SHARED};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_table);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id];

        for (auto& request : lock_request_queue.request_queue_) {
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = true;
                break;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        if (lock_request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK) {
            lock_request_queue.group_lock_mode_ = GroupLockMode::IS;
        }
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_SHARED};
        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        lock_request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    auto& request_queue = lock_table_[lock_data_id];
    bool is_ok = true;
    for (auto& request : request_queue.request_queue_) {
        if (request.txn_id_ == txn->get_transaction_id()) {
            if (request.granted_ &&
                (request.lock_mode_ == LockMode::INTENTION_EXCLUSIVE ||
                 request.lock_mode_ == LockMode::EXLUCSIVE))
                return true;
        }
        request.granted_ = false;
        is_ok = false;
        break;
    }
    if (is_ok) {
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_EXCLUSIVE};
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        bool flag = false;
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id];

        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = true;
                break;
            }
            // 表上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = true;
                break;
            }
        }
        if (flag) {
            throw TransactionAbortException(txn->get_transaction_id(),
                                            AbortReason::DEADLOCK_PREVENTION);
        }

        lock_request_queue.group_lock_mode_ = GroupLockMode::IX;
        auto& request_queue = lock_table_[lock_data_id];
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_EXCLUSIVE};
        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                if (request.lock_mode_ != LockMode::EXLUCSIVE) {
                    request.lock_mode_ = LockMode::INTENTION_EXCLUSIVE;
                }
                request.granted_ = true;
                return true;
            }
        }
        lock_request.granted_ = true;
        request_queue.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id);
        return true;
    }
    return false;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    std::unique_lock<std::mutex> lock(latch_);
    txn->set_state(TransactionState::SHRINKING);
    auto& request_queue = lock_table_[lock_data_id];
    auto lock_request = request_queue.request_queue_.begin();
    auto now = request_queue.request_queue_.end();
    for (; lock_request != request_queue.request_queue_.end(); ++lock_request) {
        if (lock_request->txn_id_ == txn->get_transaction_id()) {
            now = lock_request;
            break;
        }
    }
    if (now == request_queue.request_queue_.end())
        return false;
    request_queue.request_queue_.erase(now);
    request_queue.group_lock_mode_ = GroupLockMode::NON_LOCK;
    for (lock_request = request_queue.request_queue_.begin();
         lock_request != request_queue.request_queue_.end(); ++lock_request) {
        if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK) {
            if (lock_request->lock_mode_ == LockMode::SHARED) {
                request_queue.group_lock_mode_ = GroupLockMode::S;
                break;
            } else if (lock_request->lock_mode_ ==
                       LockMode::INTENTION_EXCLUSIVE)
                request_queue.group_lock_mode_ = GroupLockMode::IX;
            else if (lock_request->lock_mode_ == LockMode::INTENTION_SHARED)
                request_queue.group_lock_mode_ = GroupLockMode::IS;
            else if (lock_request->lock_mode_ == LockMode::EXLUCSIVE) {
                request_queue.group_lock_mode_ = GroupLockMode::X;
                break;
            } else if (lock_request->lock_mode_ == LockMode::S_IX) {
                request_queue.group_lock_mode_ = GroupLockMode::SIX;
                break;
            }
        } else if (request_queue.group_lock_mode_ == GroupLockMode::IS) {
            if (lock_request->lock_mode_ == LockMode::INTENTION_EXCLUSIVE) {
                request_queue.group_lock_mode_ = GroupLockMode::IX;
                break;
            }
        }
    }
    return true;
}