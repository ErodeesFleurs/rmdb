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
            (lock_mode == LockMode::EXCLUSIVE || 
             lock_request.lock_mode_ == LockMode::EXCLUSIVE ||
             lock_request.lock_mode_ == LockMode::GAP_EXCLUSIVE)) {
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
        lock_mode == LockMode::EXCLUSIVE ? GroupLockMode::X : GroupLockMode::S;
    txn->append_lock(lock_data_id);
    std::cerr << txn->get_transaction_id() << "lock success: " << time(NULL)
              << std::endl;
    return true;
}

/**
 * @description: 检查并获取间隙锁，如果锁被其他事务占用，应用wait-die算法处理死锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {LockDataId&} lock_data_id 加锁的目标资源ID
 * @param {LockMode} lock_mode 加锁的模式
 */
bool LockManager::CheckAndGrantGapLock(Transaction* txn,
                                          LockDataId& lock_data_id,
                                          LockMode lock_mode, std::pair<Value, Value> rg) {
    std::unique_lock<std::mutex> lock(latch_);
    auto& lock_request_queue = lock_table_[lock_data_id];

    auto in_range = [&](std::pair<Value, Value> &a, std::pair<Value, Value> &b) -> bool {
        return !(a.second < b.first || b.second < a.first);
    };

    // 检查当前加锁队列中的锁模式
    for (auto& lock_request : lock_request_queue.request_queue_) {
        // std::cerr << "LOCKGRNAD?? -> " << lock_request.granted_ << ' ' << lock_request.lock_mode_ << ' ' << lock_request.gap_rg_.first << ' ' << lock_request.gap_rg_.second << ' ' << rg.first << ' ' << rg.second << ' ' << std::endl;
        if (lock_request.granted_ &&
            (lock_request.lock_mode_ == LockMode::EXCLUSIVE ||
             lock_request.lock_mode_ == LockMode::GAP_EXCLUSIVE && in_range(lock_request.gap_rg_, rg) || 
             lock_mode == LockMode::GAP_EXCLUSIVE && (lock_request.lock_mode_ == LockMode::SHARED || lock_request.lock_mode_ == LockMode::GAP_SHARED && in_range(lock_request.gap_rg_, rg)))) {
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
    lock_request_queue.request_queue_.emplace_back(txn->get_transaction_id(), lock_mode, 
                                                   rg);
    lock_request_queue.request_queue_.back().granted_ = true;
    lock_request_queue.group_lock_mode_ =
        lock_mode == LockMode::EXCLUSIVE ? GroupLockMode::X : GroupLockMode::S;     // no use;
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
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_ = {tab_fd, rid, LockDataType::RECORD};
    // auto& request_queue_ = lock_table_[lock_data_id_];
    // bool ok = true;
    // for (auto i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         std::cout << i.txn_id_ << " " << i.lock_mode_ << " " << i.granted_
    //                   << "\n";
    //         //已有锁
    //         if (i.granted_)
    //             return true;
    //         //在等待状态
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::SHARED};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
    //     auto& lock_request_queue = lock_table_[lock_data_id_table];

    //     // 表上有X锁不能申请
    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE &&
    //             request.txn_id_ != txn->get_transaction_id() &&
    //             request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }

    //     // 行上有X锁不能申请
    //     LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
    //     auto& request_queue = lock_table_[lock_data_id];

    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE &&
    //             request.txn_id_ != txn->get_transaction_id() &&
    //             request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }
    //     if (flag) {
    //         //            lock.unlock();
    //         //            std::this_thread::sleep_for(std::chrono::microseconds(100));
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }
    //     // 行上加S锁
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::SHARED};
    //     if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK) {
    //         request_queue.group_lock_mode_ = GroupLockMode::S;
    //     }
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
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
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_ = {tab_fd, rid, LockDataType::RECORD};
    // auto& request_queue_ = lock_table_[lock_data_id_];
    // bool ok = true;
    // for (auto& i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         if (i.granted_ && i.lock_mode_ == LockMode::EXCLUSIVE) {
    //             return true;
    //         }
    //         i.granted_ = false;
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::EXCLUSIVE};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
    //     auto& lock_request_queue = lock_table_[lock_data_id_table];

    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id())
    //             continue;
    //         // 表上有X锁不能申请
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //         // 表上有S锁不能申请
    //         if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //         // 表上有SIX锁不能申请
    //         if (request.lock_mode_ == LockMode::S_IX && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }

    //     LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
    //     auto& request_queue = lock_table_[lock_data_id];
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id())
    //             continue;
    //         // 行上有S锁不能申请
    //         if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //         // 行上有X锁不能申请
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }

    //     if (flag) {
    //         //            lock.unlock();
    //         //            std::this_thread::sleep_for(std::chrono::microseconds(100));
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }
    //     // 行上加X锁
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::EXCLUSIVE};
    //     request_queue.group_lock_mode_ = GroupLockMode::X;
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             request.lock_mode_ = LockMode::EXCLUSIVE;
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
    return false;
}

/**
 * @description: 申请行级间隙读锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_shared_on_gap(Transaction* txn, const std::pair<Value, Value> rg,
                                           int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock exclusive on record"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantGapLock(txn, lock_data_id, LockMode::GAP_SHARED, rg);
}

/**
 * @description: 申请行级间隙排他锁
 * @return {bool} 加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {Rid&} rid 加锁的目标记录ID
 * @param {int} tab_fd 记录所在的表的fd
 */
bool LockManager::lock_exclusive_on_gap(Transaction* txn, const std::pair<Value, Value> rg,
                                           int tab_fd) {
    // std::cerr << txn->get_transaction_id() << " lock exclusive on record"
    //           << std::endl;
    LockDataId lock_data_id(tab_fd, LockDataType::TABLE);
    return CheckAndGrantGapLock(txn, lock_data_id, LockMode::GAP_EXCLUSIVE, rg);
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_table_ = {tab_fd, LockDataType::TABLE};
    // auto& request_queue_ = lock_table_[lock_data_id_table_];
    // bool ok = true;
    // for (auto i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         //已有锁
    //         if (i.granted_)
    //             return true;
    //         //在等待状态
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::SHARED};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_table_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
    //     auto& lock_request_queue = lock_table_[lock_data_id_table];

    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id())
    //             continue;
    //         // 表上有IX锁不能申请
    //         if (request.lock_mode_ == LockMode::INTENTION_EXCLUSIVE &&
    //             request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //         // 表上有X锁不能申请
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }

    //     if (flag) {
    //         //            lock.unlock();
    //         //            std::this_thread::sleep_for(std::chrono::microseconds(100));
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }

    //     // 表上加S锁
    //     LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    //     auto& request_queue = lock_table_[lock_data_id];
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::SHARED};
    //     if (request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK)
    //         request_queue.group_lock_mode_ = GroupLockMode::S;
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
    return false;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_ = {tab_fd, LockDataType::TABLE};
    // auto& request_queue_ = lock_table_[lock_data_id_];
    // bool ok = true;
    // for (auto i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         if (i.granted_ && i.lock_mode_ == LockMode::EXCLUSIVE) {
    //             return true;
    //         }
    //         i.granted_ = false;
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::EXCLUSIVE};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判够吧环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    //     auto& request_queue = lock_table_[lock_data_id];
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ != txn->get_transaction_id()) {
    //             //有人持有锁
    //             if (request.lock_mode_ == LockMode::EXCLUSIVE &&
    //                 request.granted_)
    //                 flag = 1;
    //             if (request.lock_mode_ == LockMode::INTENTION_EXCLUSIVE &&
    //                 request.granted_)
    //                 flag = 1;
    //             if (request.lock_mode_ == LockMode::SHARED && request.granted_)
    //                 flag = 1;
    //             if (request.lock_mode_ == LockMode::INTENTION_SHARED &&
    //                 request.granted_)
    //                 flag = 1;
    //             if (request.lock_mode_ == LockMode::S_IX && request.granted_)
    //                 flag = 1;
    //         }
    //     }
    //     if (flag) {
    //         //            lock.unlock();
    //         //            std::this_thread::sleep_for(std::chrono::microseconds(100));
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }
    //     // 表上加X锁
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::EXCLUSIVE};
    //     request_queue.group_lock_mode_ = GroupLockMode::X;
    //     for (auto& request : request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             request.lock_mode_ = LockMode::EXCLUSIVE;
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
    return false;
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_table_ = {tab_fd, LockDataType::TABLE};
    // auto& request_queue_ = lock_table_[lock_data_id_table_];
    // bool ok = true;
    // for (auto i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         //已有锁
    //         if (i.granted_)
    //             return true;
    //         //在等待状态
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::INTENTION_SHARED};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_table_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判够吧环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    //     auto& lock_request_queue = lock_table_[lock_data_id];

    //     for (auto& request : lock_request_queue.request_queue_) {
    //         // 表上有X锁不能申请
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE &&
    //             request.txn_id_ != txn->get_transaction_id() &&
    //             request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }
    //     if (flag) {
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }

    //     if (lock_request_queue.group_lock_mode_ == GroupLockMode::NON_LOCK) {
    //         lock_request_queue.group_lock_mode_ = GroupLockMode::IS;
    //     }
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::INTENTION_SHARED};
    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     lock_request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
    return false;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    // txn->set_state(TransactionState::GROWING);
    // std::unique_lock<std::mutex> lock(latch_);
    // // 获取队列
    // LockDataId lock_data_id_ = {tab_fd, LockDataType::TABLE};
    // auto& request_queue_ = lock_table_[lock_data_id_];
    // bool ok = true;
    // for (auto i : request_queue_.request_queue_) {
    //     if (i.txn_id_ == txn->get_transaction_id()) {
    //         if (i.granted_) {
    //             if (i.lock_mode_ == LockMode::INTENTION_EXCLUSIVE ||
    //                 i.lock_mode_ == LockMode::EXCLUSIVE)
    //                 return true;
    //         }
    //         i.granted_ = false;
    //         ok = false;
    //         break;
    //     }
    // }
    // if (ok) {
    //     //需要加边(进入等待队列)
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::INTENTION_EXCLUSIVE};
    //     request_queue_.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id_);
    // }
    // lock.unlock();
    // while (true) {
    //     lock.lock();
    //     //判够吧环
    //     int flag = 0;
    //     //        if (!check_loop(txn)) {
    //     //            flag = 1;
    //     //        }
    //     LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
    //     auto& lock_request_queue = lock_table_[lock_data_id];

    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id())
    //             continue;
    //         // 表上有X锁不能申请
    //         if (request.lock_mode_ == LockMode::EXCLUSIVE && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //         // 表上有S锁不能申请
    //         if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
    //             flag = 1;
    //             break;
    //         }
    //     }
    //     if (flag) {
    //         throw TransactionAbortException(txn->get_transaction_id(),
    //                                         AbortReason::DEADLOCK_PREVENTION);
    //     }

    //     lock_request_queue.group_lock_mode_ = GroupLockMode::IX;
    //     auto& request_queue = lock_table_[lock_data_id];
    //     LockRequest lock_request = {txn->get_transaction_id(),
    //                                 LockMode::INTENTION_EXCLUSIVE};
    //     for (auto& request : lock_request_queue.request_queue_) {
    //         if (request.txn_id_ == txn->get_transaction_id()) {
    //             if (request.lock_mode_ == LockMode::INTENTION_SHARED) {
    //                 request.lock_mode_ = LockMode::INTENTION_EXCLUSIVE;
    //             }
    //             request.granted_ = true;
    //             return true;
    //         }
    //     }
    //     lock_request.granted_ = true;
    //     request_queue.request_queue_.push_back(lock_request);
    //     txn->append_lock(lock_data_id);
    //     return true;
    // }
    return false;
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
    lock_request_queue.cv_.notify_all();
    return true;
}