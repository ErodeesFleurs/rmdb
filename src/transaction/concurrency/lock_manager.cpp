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
    auto& lock_request_queue = lock_table_[lock_data_id];

    // 检查当前加锁队列中的锁模式
    for (auto& lock_request : lock_request_queue.request_queue_) {
        if (lock_request.granted_ &&
            (lock_request.lock_mode_ == LockMode::EXLUCSIVE ||
             lock_mode == LockMode::EXLUCSIVE)) {
            if (txn->get_transaction_id() > lock_request.txn_id_) {
                // 当前事务优先级更低，则中止持有锁的事务
                txn->set_state(TransactionState::ABORTED);
                throw TransactionAbortException(
                    txn->get_transaction_id(),
                    AbortReason::DEADLOCK_PREVENTION);
            } else {
                // 如果当前事务优先级更高，则等待
                auto check = [&] {
                    return !lock_request.granted_ ||
                           lock_request.txn_id_ == txn->get_transaction_id();
                };
                lock_request_queue.cv_.wait(lock, check);
                break;
            }
        }
    }

    // 如果没有冲突，或者可以获取锁，则添加锁请求到队列并授予锁
    lock_request_queue.request_queue_.emplace_back(txn->get_transaction_id(),
                                                   lock_mode);
    lock_request_queue.request_queue_.back().granted_ = true;
    lock_request_queue.group_lock_mode_ =
        lock_mode == LockMode::EXLUCSIVE ? GroupLockMode::X : GroupLockMode::S;
    txn->append_lock(lock_data_id);
    std::cerr << txn->get_transaction_id() << "lock success: " << time(NULL)
              << std::endl;
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
    return CheckAndGrantIntentLock(txn, lock_data_id,
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
    return CheckAndGrantIntentLock(txn, lock_data_id,
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
    auto it = lock_table_.find(lock_data_id);
    if (it == lock_table_.end()) {
        return false;
    }

    auto& lock_request_queue = it->second;
    for (auto& lock_request : lock_request_queue.request_queue_) {
        if (lock_request.txn_id_ == txn->get_transaction_id()) {
            lock_request.granted_ = false;
        }
    }
    // auto size = lock_request_queue.request_queue_.size();
    // lock_request_queue.request_queue_.erase(
    //     std::remove_if(lock_request_queue.request_queue_.begin(),
    //                    lock_request_queue.request_queue_.end(),
    //                    [txn](const LockRequest& request) {
    //                        return request.txn_id_ == txn->get_transaction_id();
    //                    }),
    //     lock_request_queue.request_queue_.end());
    // if (size == lock_request_queue.request_queue_.size()) {
    //     return false;
    // }
    // std::cerr << txn->get_transaction_id() << " unlock success, queue size: "
    //           << lock_request_queue.request_queue_.size() << std::endl;
    lock_request_queue.cv_.notify_one();
    return true;
}