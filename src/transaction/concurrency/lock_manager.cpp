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
    std::cout << txn->get_transaction_id() << "申请行级S锁" << rid.page_no
              << " " << rid.slot_no << " " << tab_fd << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_ = {tab_fd, rid, LockDataType::RECORD};
    auto& request_queue_ = lock_table_[lock_data_id_];
    bool ok = true;
    for (auto i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            //已有锁
            if (i.granted_)
                return true;
            //在等待状态
            ok = false;
            break;
        }
    }
    if (ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::SHARED};
        request_queue_.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        //判环
        int flag = 0;
        if (!check_loop(txn)) {
            flag = 1;
        }
        LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id_table];

        // 表上有X锁不能申请
        for (auto& request : lock_request_queue.request_queue_) {
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = 1;
                break;
            }
        }

        // 行上有X锁不能申请
        LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
        auto& request_queue = lock_table_[lock_data_id];

        for (auto& request : request_queue.request_queue_) {
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
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
    std::cout << txn->get_transaction_id() << "申请行级X锁" << rid.page_no
              << " " << rid.slot_no << " " << tab_fd << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_ = {tab_fd, rid, LockDataType::RECORD};
    auto& request_queue_ = lock_table_[lock_data_id_];
    bool ok = true;
    for (auto& i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            if (i.granted_ && i.lock_mode_ == LockMode::EXLUCSIVE) {
                return true;
            }
            i.granted_ = false;
            ok = false;
            break;
        }
    }
    if (ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue_.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        //判环
        int flag = 0;
        if (!check_loop(txn)) {
            flag = 1;
        }
        LockDataId lock_data_id_table = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id_table];

        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = 1;
                break;
            }
            // 表上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = 1;
                break;
            }
            // 表上有SIX锁不能申请
            if (request.lock_mode_ == LockMode::S_IX && request.granted_) {
                flag = 1;
                break;
            }
        }

        LockDataId lock_data_id = {tab_fd, rid, LockDataType::RECORD};
        auto& request_queue = lock_table_[lock_data_id];
        for (auto& request : request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 行上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = 1;
                break;
            }
            // 行上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = 1;
                break;
            }
        }

        if (flag) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
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
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::cout << txn->get_transaction_id() << "申请表级S锁" << " " << tab_fd
              << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_table_ = {tab_fd, LockDataType::TABLE};
    auto& request_queue_ = lock_table_[lock_data_id_table_];
    for (auto i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            //已有锁
            if (i.granted_)
                return true;
            //在等待状态
            assert(1);
        }
    }
    //需要加边(进入等待队列)
    LockRequest lock_request = {txn->get_transaction_id(), LockMode::SHARED};
    request_queue_.request_queue_.push_back(lock_request);
    txn->append_lock_set(lock_data_id_table_);
    while (true) {
        //判环
        int flag = 0;
        for (auto i : request_queue_.request_queue_) {
            if (i.granted_ && i.lock_mode_ == LockMode::EXLUCSIVE) {
                // 申请的年轻就滚蛋
                if (i.txn_id_ < txn->get_transaction_id()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    throw TransactionAbortException(
                        txn->get_transaction_id(),
                        AbortReason::DEADLOCK_PREVENTION);
                }
            }
        }
        txn_id_t tt;
        for (auto& request : request_queue_.request_queue_) {
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = 1;
                tt = request.txn_id_;
                break;
            }
        }
        if (flag) {
            assert(txn->get_transaction_id() < tt);
            //            std::cout << txn->get_transaction_id() << "等待" << tt << '\n';
            request_queue_.cv_.wait(lock);
            continue;
        }

        // 表上加S锁
        for (auto& request : request_queue_.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.granted_ = true;
                //                std::cout << txn->get_transaction_id() << "申请表级S锁成功" << " " << tab_fd << '\n';
                return true;
            }
        }
        assert(1);
    }
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::cout << txn->get_transaction_id() << "申请表级X锁" << " " << tab_fd
              << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_ = {tab_fd, LockDataType::TABLE};
    auto& request_queue_ = lock_table_[lock_data_id_];
    bool ok = true;
    for (auto i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            if (i.granted_ && i.lock_mode_ == LockMode::EXLUCSIVE) {
                return true;
            }
            ok = false;
            break;
        }
    }
    if (ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::EXLUCSIVE};
        request_queue_.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_);
    }
    while (true) {
        //判环
        int flag = 0;
        for (auto i : request_queue_.request_queue_) {
            if (i.granted_) {
                // 申请的年轻就滚蛋
                if (i.txn_id_ < txn->get_transaction_id()) {
                    //                    std::cout << "锁在" << i.txn_id_ << "手上111\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    throw TransactionAbortException(
                        txn->get_transaction_id(),
                        AbortReason::DEADLOCK_PREVENTION);
                }
            }
        }
        txn_id_t mn = -1;
        txn_id_t tt = 99999999;
        for (auto& request : request_queue_.request_queue_) {
            if (mn == -1)
                mn = request.txn_id_;
            else
                mn = std::min(mn, request.txn_id_);
            if (request.txn_id_ != txn->get_transaction_id()) {
                //有人持有锁
                if (request.granted_) {
                    flag = 1;
                    tt = request.txn_id_;
                    break;
                }
            }
        }
        if (flag || mn != txn->get_transaction_id()) {
            assert(txn->get_transaction_id() < tt);
            //            std::cout << txn->get_transaction_id() << "等待" << tt << '\n';
            request_queue_.cv_.wait(lock);
            continue;
        }
        // 表上加X锁
        for (auto& request : request_queue_.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id()) {
                request.lock_mode_ = LockMode::EXLUCSIVE;
                request.granted_ = true;
                //                std::cout << txn->get_transaction_id() << "申请表级X锁成功" << " " << tab_fd << '\n';
                return true;
            }
        }
        assert(1);
    }
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);

    std::cout << txn->get_transaction_id() << "申请表级IS锁" << " " << tab_fd
              << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_table_ = {tab_fd, LockDataType::TABLE};
    auto& request_queue_ = lock_table_[lock_data_id_table_];
    bool ok = true;
    for (auto i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            //已有锁
            if (i.granted_)
                return true;
            //在等待状态
            ok = false;
            break;
        }
    }
    if (ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_SHARED};
        request_queue_.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_table_);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        //判环
        int flag = 0;
        if (!check_loop(txn)) {
            flag = 1;
        }
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id];

        for (auto& request : lock_request_queue.request_queue_) {
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE &&
                request.txn_id_ != txn->get_transaction_id() &&
                request.granted_) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
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
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {
    txn->set_state(TransactionState::GROWING);
    std::cout << txn->get_transaction_id() << "申请表级IX锁" << " " << tab_fd
              << '\n';
    std::unique_lock<std::mutex> lock(latch_);
    // 获取队列
    LockDataId lock_data_id_ = {tab_fd, LockDataType::TABLE};
    auto& request_queue_ = lock_table_[lock_data_id_];
    bool ok = true;
    for (auto i : request_queue_.request_queue_) {
        if (i.txn_id_ == txn->get_transaction_id()) {
            if (i.granted_) {
                if (i.lock_mode_ == LockMode::INTENTION_EXCLUSIVE ||
                    i.lock_mode_ == LockMode::EXLUCSIVE)
                    return true;
            }
            i.granted_ = false;
            ok = false;
            break;
        }
    }
    if (ok) {
        //需要加边(进入等待队列)
        LockRequest lock_request = {txn->get_transaction_id(),
                                    LockMode::INTENTION_EXCLUSIVE};
        request_queue_.request_queue_.push_back(lock_request);
        txn->append_lock_set(lock_data_id_);
    }
    lock.unlock();
    while (true) {
        lock.lock();
        //判环
        int flag = 0;
        if (!check_loop(txn)) {
            flag = 1;
        }
        LockDataId lock_data_id = {tab_fd, LockDataType::TABLE};
        auto& lock_request_queue = lock_table_[lock_data_id];

        for (auto& request : lock_request_queue.request_queue_) {
            if (request.txn_id_ == txn->get_transaction_id())
                continue;
            // 表上有X锁不能申请
            if (request.lock_mode_ == LockMode::EXLUCSIVE && request.granted_) {
                flag = 1;
                break;
            }
            // 表上有S锁不能申请
            if (request.lock_mode_ == LockMode::SHARED && request.granted_) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
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
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {
    std::unique_lock<std::mutex> lock(latch_);
    std::cout << txn->get_transaction_id() << "释放锁  " << lock_data_id.fd_
              << '\n';
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
    if (request_queue.request_queue_.empty())
        lock_table_.erase(lock_data_id);
    request_queue.cv_.notify_all();
    //    std::cout << txn->get_transaction_id() << "释放锁成功  "  << lock_data_id.fd_ << '\n';
    return true;
}

bool LockManager::check_loop(Transaction* txn) {
    //    std::cout << "start_check\n";
    int tot = 0;
    std::unordered_map<txn_id_t, int> mp;
    std::unordered_map<int, txn_id_t> rmp;
    for (auto& i : lock_table_) {
        for (auto j : i.second.request_queue_) {
            if (!mp.count(j.txn_id_)) {
                mp[j.txn_id_] = tot;
                rmp[tot++] = j.txn_id_;
            }
        }
    }
    std::vector<std::vector<int>> e(tot);
    std::vector<int> du(tot), que(tot);
    int front = 0, end = 0;
    for (auto& i : lock_table_) {
        std::vector<int> granted, un_granted;
        for (auto j : i.second.request_queue_) {
            if (j.granted_) {
                granted.push_back(mp[j.txn_id_]);
            } else {
                un_granted.push_back(mp[j.txn_id_]);
            }
        }
        for (auto u : un_granted) {
            for (auto v : granted) {
                e[u].push_back(v);
                //                std::cout << u << " -> " << v << "\n";
                du[v]++;
            }
        }
    }
    for (int i = 0; i < tot; i++) {
        if (!du[i])
            que[end++] = i;
    }
    while (front < end) {
        int u = que[front++];
        for (auto v : e[u]) {
            du[v]--;
            if (!du[v])
                que[end++] = v;
        }
    }
    txn_id_t mx = -1;
    for (int i = 0; i < tot; i++) {
        if (du[i]) {
            mx = std::max(mx, rmp[i]);
        }
    }
    if (mx != -1) {
        if (txn->get_transaction_id() == mx) {
            throw TransactionAbortException(mx,
                                            AbortReason::DEADLOCK_PREVENTION);
        }
        return false;
    }
    return true;
}