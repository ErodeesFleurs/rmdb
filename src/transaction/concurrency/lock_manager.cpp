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
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

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
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, rid, LockDataType::RECORD);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

    return true;
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(), LockMode::SHARED);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

    return true;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(), LockMode::EXLUCSIVE);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

    return true;
}
/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(), LockMode::INTENTION_SHARED);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

    return true;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    std::unique_lock<std::mutex> lock(latch_);
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    auto& request_queue = lock_table_[lock_data_id];
    LockRequest request(txn->get_transaction_id(),
                        LockMode::INTENTION_EXCLUSIVE);
    request_queue.request_queue_.push_back(request);

    if (!WaitDie(request_queue, request)) {
        while (!GrantLock(request_queue, request)) {
            request_queue.cv_.wait(lock);
        }
    }

    return true;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    std::unique_lock<std::mutex> lock(latch_);
    auto it = lock_table_.find(lock_data_id);
    if (it == lock_table_.end()) {
        return false;
    }

    auto& request_queue = it->second;
    auto request_it = std::find_if(
        request_queue.request_queue_.begin(),
        request_queue.request_queue_.end(), [&](const LockRequest& req) {
            return req.txn_id_ == txn->get_transaction_id();
        });

    if (request_it == request_queue.request_queue_.end()) {
        return false;
    }

    request_queue.request_queue_.erase(request_it);
    request_queue.group_lock_mode_ = CalculateGroupLockMode(request_queue);
    request_queue.cv_.notify_all();

    if (request_queue.request_queue_.empty()) {
        lock_table_.erase(it);
    }
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

bool LockManager::GrantLock(LockRequestQueue& request_queue,
                            LockRequest& request) {
    if (CanGrantLock(request_queue, request.lock_mode_)) {
        request.granted_ = true;
        request_queue.group_lock_mode_ = CalculateGroupLockMode(request_queue);
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
    bool hasS = false, hasX = false, hasIS = false, hasIX = false;
    for (const auto& request : request_queue.request_queue_) {
        switch (request.lock_mode_) {
            case LockMode::SHARED:
                hasS = true;
                break;
            case LockMode::EXLUCSIVE:
                hasX = true;
                break;
            case LockMode::INTENTION_SHARED:
                hasIS = true;
                break;
            case LockMode::INTENTION_EXCLUSIVE:
                hasIX = true;
                break;
            case LockMode::S_IX:
                hasS = true;
                hasIX = true;
                break;
        }
    }
    if (hasX)
        return GroupLockMode::X;
    if (hasS && hasIX)
        return GroupLockMode::SIX;
    if (hasS)
        return GroupLockMode::S;
    if (hasIX)
        return GroupLockMode::IX;
    if (hasIS)
        return GroupLockMode::IS;
    return GroupLockMode::NON_LOCK;
}

bool LockManager::WaitDie(LockRequestQueue& request_queue,
                          LockRequest& request) {
    for (const auto& req : request_queue.request_queue_) {
        if (req.granted_ && req.txn_id_ < request.txn_id_) {
            return true;
        } else if (req.granted_ && req.txn_id_ > request.txn_id_) {
            throw TransactionAbortException(request.txn_id_,
                                            AbortReason::DEADLOCK_PREVENTION);
        }
    }
    return false;
}