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

    return false;
}

/**
 * @description: 申请表级读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_shared_on_table(Transaction* txn, int tab_fd) {

    return false;
}

/**
 * @description: 申请表级写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_exclusive_on_table(Transaction* txn, int tab_fd) {

    return false;
}

/**
 * @description: 申请表级意向读锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IS_on_table(Transaction* txn, int tab_fd) {

    return false;
}

/**
 * @description: 申请表级意向写锁
 * @return {bool} 返回加锁是否成功
 * @param {Transaction*} txn 要申请锁的事务对象指针
 * @param {int} tab_fd 目标表的fd
 */
bool LockManager::lock_IX_on_table(Transaction* txn, int tab_fd) {

    return false;
}

/**
 * @description: 释放锁
 * @return {bool} 返回解锁是否成功
 * @param {Transaction*} txn 要释放锁的事务对象指针
 * @param {LockDataId} lock_data_id 要释放的锁ID
 */
bool LockManager::unlock(Transaction* txn, LockDataId lock_data_id) {

    return true;
}

bool LockManager::check_loop(Transaction* txn) {
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
                std::cout << u << " -> " << v << "\n";
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