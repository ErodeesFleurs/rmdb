/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "ix_index_handle.h"

#include "ix_scan.h"

/**
 * @brief 在当前node中查找第一个>=target的key_idx
 *
 * @return key_idx，范围为[0,num_key)，如果返回的key_idx=num_key，则表示target大于最后一个key
 * @note 返回key index（同时也是rid index），作为slot no
 */
int IxNodeHandle::lower_bound(const char* target) const {
    // Todo:
    // 查找当前节点中第一个大于等于target的key，并返回key的位置给上层
    // 提示: 可以采用多种查找方式，如顺序遍历、二分查找等；使用ix_compare()函数进行比较
    int l = 0, r = page_hdr->num_key - 1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        char* now = get_key(mid);
        int res =
            ix_compare(target, now, file_hdr->col_types_, file_hdr->col_lens_);
        if (res <= 0) {  // target <= now
            r = mid - 1;
        } else {  // target > now
            l = mid + 1;
        }
    }
    return l;
}

/**
 * @brief 在当前node中查找第一个>target的key_idx
 *
 * @return key_idx，范围为[0,num_key)，如果返回的key_idx=num_key，则表示target大于等于最后一个key
 * @note 注意此处的范围从1开始
 */
int IxNodeHandle::upper_bound(const char* target) const {
    // Todo:
    // 查找当前节点中第一个大于target的key，并返回key的位置给上层
    // 提示: 可以采用多种查找方式：顺序遍历、二分查找等；使用ix_compare()函数进行比较
    int l = 0, r = get_size() - 1;
    while (l <= r) {
        int mid = (l + r) >> 1;
        char* now = get_key(mid);
        int res =
            ix_compare(target, now, file_hdr->col_types_, file_hdr->col_lens_);
        if (res < 0) {  // target < now
            r = mid - 1;
        } else {  // target >= now
            l = mid + 1;
        }
    }
    return l;
}

/**
 * @brief 用于叶子结点根据key来查找该结点中的键值对
 * 值value作为传出参数，函数返回是否查找成功
 *
 * @param key 目标key
 * @param[out] value 传出参数，目标key对应的Rid
 * @return 目标key是否存在
 */
bool IxNodeHandle::leaf_lookup(const char* key, Rid** value) {
    // Todo:
    // 1. 在叶子节点中获取目标key所在位置
    // 2. 判断目标key是否存在
    // 3. 如果存在，获取key对应的Rid，并赋值给传出参数value
    // 提示：可以调用lower_bound()和get_rid()函数。
    assert(is_leaf_page());
    int key_idx = lower_bound(key);
    if (key_idx == get_size()) {
        //不存在
        return false;
    }
    char* now = get_key(key_idx);
    int res = ix_compare(key, now, file_hdr->col_types_, file_hdr->col_lens_);
    if (res != 0) {
        //不相等
        return false;
    }
    *value = get_rid(key_idx);
    return true;
}

/**
 * 用于内部结点（非叶子节点）查找目标key所在的孩子结点（子树）
 * @param key 目标key
 * @return page_id_t 目标key所在的孩子节点（子树）的存储页面编号
 */
page_id_t IxNodeHandle::internal_lookup(const char* key) {
    // Todo:
    // 1. 查找当前非叶子节点中目标key所在孩子节点（子树）的位置
    // 2. 获取该孩子节点（子树）所在页面的编号
    // 3. 返回页面编号
    int key_idx = upper_bound(key) - 1;
    key_idx = std::max(key_idx, 0);
    return value_at(key_idx);
}

/**
 * @brief 在指定位置插入n个连续的键值对
 * 将key的前n位插入到原来keys中的pos位置；将rid的前n位插入到原来rids中的pos位置
 *
 * @param pos 要插入键值对的位置
 * @param (key, rid) 连续键值对的起始地址，也就是第一个键值对，可以通过(key, rid)来获取n个键值对
 * @param n 键值对数量
 * @note [0,pos)           [pos,num_key)
 *                            key_slot
 *                            /      \
 *                           /        \
 *       [0,pos)     [pos,pos+n)   [pos+n,num_key+n)
 *                      key           key_slot
 */
void IxNodeHandle::insert_pairs(int pos, const char* key, const Rid* rid,
                                int n) {
    // Todo:
    // 1. 判断pos的合法性
    // 2. 通过key获取n个连续键值对的key值，并把n个key值插入到pos位置
    // 3. 通过rid获取n个连续键值对的rid值，并把n个rid值插入到pos位置
    // 4. 更新当前节点的键数量
    if (pos >= 0 && pos <= page_hdr->num_key) {
        auto kp = get_key(pos);
        memmove(kp + n * file_hdr->col_tot_len_, kp,
                (page_hdr->num_key - pos) * file_hdr->col_tot_len_);
        memcpy(kp, key, n * file_hdr->col_tot_len_);
        auto rp = get_rid(pos);
        memmove(rp + n, rp, (page_hdr->num_key - pos) * sizeof(Rid));
        memcpy(rp, rid, n * sizeof(Rid));
        page_hdr->num_key += n;
    }
}

/**
 * @brief 用于在结点中插入单个键值对。
 * 函数返回插入后的键值对数量
 *
 * @param (key, value) 要插入的键值对
 * @return int 键值对数量
 */
int IxNodeHandle::insert(const char* key, const Rid& value) {
    // Todo:
    // 1. 查找要插入的键值对应该插入到当前节点的哪个位置
    // 2. 如果key重复则不插入
    // 3. 如果key不重复则插入键值对
    // 4. 返回完成插入操作之后的键值对数量
    int pos = lower_bound(key);
    if (pos < get_size()) {
        char* now = get_key(pos);
        int res =
            ix_compare(key, now, file_hdr->col_types_, file_hdr->col_lens_);
        if (res == 0) {
            //key重复则不插入
            return page_hdr->num_key;
        }
    }
    //说明key不重复，则在pos插入键值对
    insert_pair(pos, key, value);
    return page_hdr->num_key;
}

/**
 * @brief 用于在结点中的指定位置删除连续n个键值对
 *
 * @param pos 要删除键值对的起始位置
 */
void IxNodeHandle::erase_pairs(int pos, int n) {
    // 1. 删除该位置的key
    // 2. 删除该位置的rid
    // 3. 更新结点的键值对数量
    if (pos >= 0 && pos < page_hdr->num_key) {
        auto kp = get_key(pos);
        memmove(kp, kp + n * file_hdr->col_tot_len_,
                (page_hdr->num_key - pos - n) * file_hdr->col_tot_len_);
        auto rp = get_rid(pos);
        memmove(rp, rp + n, (page_hdr->num_key - pos - n) * sizeof(Rid));
        page_hdr->num_key -= n;
    }
}

/**
 * @brief 用于在结点中删除指定key的键值对。函数返回删除后的键值对数量
 *
 * @param key 要删除的键值对key值
 * @return 完成删除操作后的键值对数量
 */
int IxNodeHandle::remove(const char* key) {
    // Todo:
    // 1. 查找要删除键值对的位置
    // 2. 如果要删除的键值对存在，删除键值对
    // 3. 返回完成删除操作后的键值对数量
    int key_idx = lower_bound(key);
    if (key_idx == get_size()) {
        //不存在
        return page_hdr->num_key;
    }
    char* now = get_key(key_idx);
    int res = ix_compare(key, now, file_hdr->col_types_, file_hdr->col_lens_);
    if (res != 0) {
        //不相等
        return page_hdr->num_key;
    }
    erase_pair(key_idx);
    return page_hdr->num_key;
}

IxIndexHandle::IxIndexHandle(DiskManager* disk_manager,
                             BufferPoolManager* buffer_pool_manager, int fd)
    : disk_manager_(disk_manager),
      buffer_pool_manager_(buffer_pool_manager),
      fd_(fd) {
    // init file_hdr_
    disk_manager_->read_page(fd, IX_FILE_HDR_PAGE, (char*)&file_hdr_,
                             sizeof(file_hdr_));
    char* buf = new char[PAGE_SIZE];
    memset(buf, 0, PAGE_SIZE);
    disk_manager_->read_page(fd, IX_FILE_HDR_PAGE, buf, PAGE_SIZE);
    file_hdr_ = new IxFileHdr();
    file_hdr_->deserialize(buf);

    // disk_manager管理的fd对应的文件中，设置从file_hdr_->num_pages开始分配page_no
    int now_page_no = disk_manager_->get_fd2pageno(fd);
    disk_manager_->set_fd2pageno(fd, now_page_no + 1);
}

/**
 * @brief 用于查找指定键所在的叶子结点
 * @param key 要查找的目标key值
 * @param operation 查找到目标键值对后要进行的操作类型
 * @param transaction 事务参数，如果不需要则默认传入nullptr
 * @return [leaf node] and [root_is_latched] 返回目标叶子结点以及根结点是否加锁
 * @note need to Unlatch and unpin the leaf node outside!
 * 注意：用了FindLeafPage之后一定要unlatch叶结点，否则下次latch该结点会堵塞！
 */
std::pair<IxNodeHandle*, bool> IxIndexHandle::find_leaf_page(
    const char* key, Operation operation, Transaction* transaction,
    bool find_first) {
    // Todo:
    // 1. 获取根节点
    // 2. 从根节点开始不断向下查找目标key
    // 3. 找到包含该key值的叶子结点停止查找，并返回叶子节点
    auto root = fetch_node(file_hdr_->root_page_);
    while (!root->page_hdr->is_leaf) {
        auto nex = fetch_node(root->internal_lookup(key));
        buffer_pool_manager_->unpin_page(root->get_page_id(), true);
        delete root;
        root = nex;
    }
    return {root, false};
}

/**
 * @brief 用于查找指定键在叶子结点中的对应的值result
 *
 * @param key 查找的目标key值
 * @param result 用于存放结果的容器
 * @param transaction 事务指针
 * @return bool 返回目标键值对是否存在
 */
bool IxIndexHandle::get_value(const char* key, std::vector<Rid>* result,
                              Transaction* transaction) {
    // Todo:
    // 1. 获取目标key值所在的叶子结点
    // 2. 在叶子节点中查找目标key值的位置，并读取key对应的rid
    // 3. 把rid存入result参数中
    // 提示：使用完buffer_pool提供的page之后，记得unpin page；记得处理并发的上锁
    std::scoped_lock lock{root_latch_};
    auto leaf = find_leaf_page(key, Operation::FIND, nullptr, false).first;
    if (leaf == nullptr)
        return false;
    Rid* lt;
    if (leaf->leaf_lookup(key, &lt)) {
        std::vector<Rid> ans(1);
        ans[0] = (*lt);
        *result = ans;
        buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
        return true;
    }
    buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
    return false;
}

/**
 * @brief  将传入的一个node拆分(Split)成两个结点，在node的右边生成一个新结点new node
 * @param node 需要拆分的结点
 * @return 拆分得到的new_node
 * @note need to unpin the new node outside
 * 注意：本函数执行完毕后，原node和new node都需要在函数外面进行unpin
 */
IxNodeHandle* IxIndexHandle::split(IxNodeHandle* node) {
    // Todo:
    // 1. 将原结点的键值对平均分配，右半部分分裂为新的右兄弟结点
    //    需要初始化新节点的page_hdr内容
    // 2. 如果新的右兄弟结点是叶子结点，更新新旧节点的prev_leaf和next_leaf指针
    //    为新节点分配键值对，更新旧节点的键值对数记录
    // 3. 如果新的右兄弟结点不是叶子结点，更新该结点的所有孩子结点的父节点信息(使用IxIndexHandle::maintain_child())
    int left = node->get_min_size(), right = node->get_size() - left;
    auto rt = create_node();
    memcpy(rt->page_hdr, node->page_hdr, sizeof(IxPageHdr));
    //为新节点分配键值对，更新旧节点的键值对数记录
    rt->set_size(0);
    auto key = node->get_key(left);
    auto rid = node->get_rid(left);
    rt->insert_pairs(0, key, rid, right);
    //delete
    node->set_size(left);
    if (node->is_leaf_page()) {
        //是叶子节点
        //更新新旧节点的prev_leaf和next_leaf指针
        auto nex = fetch_node(node->get_next_leaf());
        // node -> nex
        // node <- nex
        // node -> rt -> nex
        // node <- rt <- nex
        nex->set_prev_leaf(rt->get_page_no());
        delete nex;
        rt->page_hdr->prev_leaf = node->get_page_no();
        node->page_hdr->next_leaf = rt->get_page_no();
    } else {
        //不是叶子，更新该结点的所有孩子结点的父节点信息
        for (int i = 0; i < right; i++) {
            maintain_child(rt, i);
        }
    }
    return rt;
}

/**
 * @brief Insert key & value pair into internal page after split
 * 拆分(Split)后，向上找到old_node的父结点
 * 将new_node的第一个key插入到父结点，其位置在 父结点指向old_node的孩子指针 之后
 * 如果插入后>=maxsize，则必须继续拆分父结点，然后在其父结点的父结点再插入，即需要递归
 * 直到找到的old_node为根结点时，结束递归（此时将会新建一个根R，关键字为key，old_node和new_node为其孩子）
 *
 * @param (old_node, new_node) 原结点为old_node，old_node被分裂之后产生了新的右兄弟结点new_node
 * @param key 要插入parent的key
 * @note 一个结点插入了键值对之后需要分裂，分裂后左半部分的键值对保留在原结点，在参数中称为old_node，
 * 右半部分的键值对分裂为新的右兄弟节点，在参数中称为new_node（参考Split函数来理解old_node和new_node）
 * @note 本函数执行完毕后，new node和old node都需要在函数外面进行unpin
 */
void IxIndexHandle::insert_into_parent(IxNodeHandle* old_node, const char* key,
                                       IxNodeHandle* new_node,
                                       Transaction* transaction) {
    // Todo:
    // 1. 分裂前的结点（原结点, old_node）是否为根结点，如果为根结点需要分配新的root
    // 2. 获取原结点（old_node）的父亲结点
    // 3. 获取key对应的rid，并将(key, rid)插入到父亲结点
    // 4. 如果父亲结点仍需要继续分裂，则进行递归插入
    // 提示：记得unpin page
    if (old_node->is_root_page()) {
        //分裂的是root, 需要新建一个root
        auto new_root = create_node();
        memcpy(new_root->page_hdr, old_node->page_hdr, sizeof(IxNodeHandle));
        //设置新root信息
        new_root->set_size(0);
        new_root->page_hdr->is_leaf = false;
        //修改父节点信息
        old_node->set_parent_page_no(new_root->get_page_no());
        new_node->set_parent_page_no(new_root->get_page_no());
        //键值对插入到新root中
        new_root->insert(old_node->get_key(0),
                         {.page_no = old_node->get_page_no(), .slot_no = -1});
        new_root->insert(key,
                         {.page_no = new_node->get_page_no(), .slot_no = -1});
        file_hdr_->root_page_ = new_root->get_page_no();
        buffer_pool_manager_->unpin_page(new_root->get_page_id(), true);
    } else {
        auto fa = fetch_node(old_node->get_parent_page_no());
        new_node->set_parent_page_no(fa->get_page_no());
        int cnt = fa->insert(
            key, {.page_no = new_node->get_page_no(), .slot_no = -1});
        if (cnt == fa->get_max_size()) {
            //split
            auto rt = split(fa);
            insert_into_parent(fa, rt->get_key(0), rt, transaction);
            buffer_pool_manager_->unpin_page(rt->get_page_id(), true);
        }
        buffer_pool_manager_->unpin_page(fa->get_page_id(), true);
        delete fa;
    }
}

/**
 * @brief 将指定键值对插入到B+树中
 * @param (key, value) 要插入的键值对
 * @param transaction 事务指针
 * @return page_id_t 插入到的叶结点的page_no
 */
std::pair<page_id_t, bool> IxIndexHandle::insert_entry(
    const char* key, const Rid& value, Transaction* transaction) {
    // Todo:
    // 1. 查找key值应该插入到哪个叶子节点
    // 2. 在该叶子节点中插入键值对
    // 3. 如果结点已满，分裂结点，并把新结点的相关信息插入父节点
    // 提示：记得unpin page；若当前叶子节点是最右叶子节点，则需要更新file_hdr_.last_leaf；记得处理并发的上锁
    std::scoped_lock lock{root_latch_};
    auto leaf = find_leaf_page(key, Operation::FIND, transaction).first;
    int old_cnt = leaf->get_size();
    // 插入数据
    int cnt = leaf->insert(key, value);
    if (old_cnt == cnt) {
        //说明插入失败
        return {leaf->get_page_id().page_no, false};
    }
    //插入后更新父节点键值
    maintain_parent(leaf);
    int pos = leaf->lower_bound(key), res = -1;
    if (cnt == leaf->get_max_size()) {
        //split
        auto node = split(leaf);
        insert_into_parent(leaf, node->get_key(0), node, transaction);
        if (file_hdr_->last_leaf_ == leaf->get_page_no()) {
            //当前叶子节点是最右叶子节点
            file_hdr_->last_leaf_ = node->get_page_no();
            //更新表头
            auto header = fetch_node(IX_LEAF_HEADER_PAGE);
            header->set_prev_leaf(node->get_page_no());
            buffer_pool_manager_->unpin_page(header->get_page_id(), true);
            delete header;
        }
        if (pos < leaf->get_size()) {
            //说明插入的是左边
            res = leaf->get_page_no();
        } else {
            //说明插入的是右边
            res = node->get_page_no();
        }
        buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
        buffer_pool_manager_->unpin_page(node->get_page_id(), true);
    } else {
        res = leaf->get_page_no();
        buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
    }
    return {res, true};
}

bool IxIndexHandle::check_entry(const char* key, Transaction* transaction) {
    auto leaf = find_leaf_page(key, Operation::FIND, transaction).first;
    auto pos = leaf->lower_bound(key);
    if (pos < leaf->get_size()) {
        char* now = leaf->get_key(pos);
        int res = ix_compare(key, now, leaf->file_hdr->col_types_,
                             leaf->file_hdr->col_lens_);
        if (res == 0) {
            //key重复
            return true;
        }
    }
    return false;
}

/**
 * @brief 用于删除B+树中含有指定key的键值对
 * @param key 要删除的key值
 * @param transaction 事务指针
 */
bool IxIndexHandle::delete_entry(const char* key, Transaction* transaction) {
    // Todo:
    // 1. 获取该键值对所在的叶子结点
    // 2. 在该叶子结点中删除键值对
    // 3. 如果删除成功需要调用CoalesceOrRedistribute来进行合并或重分配操作，并根据函数返回结果判断是否有结点需要删除
    // 4. 如果需要并发，并且需要删除叶子结点，则需要在事务的delete_page_set中添加删除结点的对应页面；记得处理并发的上锁
    std::scoped_lock lock{root_latch_};
    auto leaf = find_leaf_page(key, Operation::FIND, transaction, false).first;
    int old_cnt = leaf->get_size();
    int idx = leaf->lower_bound(key);
    int now_cnt = leaf->remove(key);
    if (old_cnt != now_cnt) {
        // 说明删除了一个键值对
        if (!idx)
            maintain_parent(leaf);
        if (!coalesce_or_redistribute(leaf)) {
            buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
        }
        return true;
    }
    buffer_pool_manager_->unpin_page(leaf->get_page_id(), true);
    return false;
}

/**
 * @brief 用于处理合并和重分配的逻辑，用于删除键值对后调用
 *
 * @param node 执行完删除操作的结点
 * @param transaction 事务指针
 * @param root_is_latched 传出参数：根节点是否上锁，用于并发操作
 * @return 是否需要删除结点
 * @note User needs to first find the sibling of input page.
 * If sibling's size + input page's size >= 2 * page's minsize, then redistribute.
 * Otherwise, merge(Coalesce).
 */
bool IxIndexHandle::coalesce_or_redistribute(IxNodeHandle* node,
                                             Transaction* transaction,
                                             bool* root_is_latched) {
    // Todo:
    // 1. 判断node结点是否为根节点
    //    1.1 如果是根节点，需要调用AdjustRoot() 函数来进行处理，返回根节点是否需要被删除
    //    1.2 如果不是根节点，并且不需要执行合并或重分配操作，则直接返回false，否则执行2
    // 2. 获取node结点的父亲结点
    // 3. 寻找node结点的兄弟结点（优先选取前驱结点）
    // 4. 如果node结点和兄弟结点的键值对数量之和，能够支撑两个B+树结点（即node.size+neighbor.size >=
    // NodeMinSize*2)，则只需要重新分配键值对（调用Redistribute函数）
    // 5. 如果不满足上述条件，则需要合并两个结点，将右边的结点合并到左边的结点（调用Coalesce函数）
    if (node->get_size() >= node->get_min_size() &&
        node->get_size() <= node->get_max_size()) {
        //点数达到要求
        return false;
    }
    if (node->is_root_page()) {
        //根节点
        if (adjust_root(node)) {
            if (node->is_leaf_page()) {
                erase_leaf(node);
            }
            release_node_handle(*node);
            buffer_pool_manager_->unpin_page(node->get_page_id(), true);
            free(node);
            return true;
        }
        buffer_pool_manager_->unpin_page(node->get_page_id(), true);
        return false;
    }
    //获取父节点
    auto fa = fetch_node(node->get_parent_page_no());
    int pos = fa->find_child(node), idx = pos - 1;
    if (idx < 0)
        idx = pos + 1;
    auto neighbor = fetch_node(fa->get_rid(idx)->page_no);
    bool ok = false;
    if (node->get_size() + neighbor->get_size() >= node->get_min_size() * 2) {
        //重分配
        redistribute(neighbor, node, fa, pos - idx);
        buffer_pool_manager_->unpin_page(neighbor->get_page_id(), true);
        buffer_pool_manager_->unpin_page(fa->get_page_id(), true);
    } else {
        //合并
        if (coalesce(&neighbor, &node, &fa, pos - idx, transaction,
                     root_is_latched)) {
            if (!coalesce_or_redistribute(fa)) {
                buffer_pool_manager_->unpin_page(fa->get_page_id(), true);
            }
        }
        if (pos > idx) {  // node在右边, 说明node被删
            buffer_pool_manager_->unpin_page(neighbor->get_page_id(), true);
            ok = true;
        }
    }
    // delete fa;
    // delete neighbor;
    return ok;
}

/**
 * @brief 用于当根结点被删除了一个键值对之后的处理
 * @param old_root_node 原根节点
 * @return bool 根结点是否需要被删除
 * @note size of root page can be less than min size and this method is only called within coalesce_or_redistribute()
 */
bool IxIndexHandle::adjust_root(IxNodeHandle* old_root_node) {
    // Todo:
    // 1. 如果old_root_node是内部结点，并且大小为1，则直接把它的孩子更新成新的根结点
    // 2. 如果old_root_node是叶结点，且大小为0，则直接更新root page
    // 3. 除了上述两种情况，不需要进行操作
    if (old_root_node->is_leaf_page() && old_root_node->get_size() == 0) {
        old_root_node->page_hdr->next_leaf = IX_LEAF_HEADER_PAGE;
        old_root_node->page_hdr->prev_leaf = IX_LEAF_HEADER_PAGE;
        old_root_node->page_hdr->parent = IX_NO_PAGE;
        old_root_node->page_hdr->next_free_page_no = IX_NO_PAGE;
        return false;
    }
    if (!old_root_node->is_leaf_page() && old_root_node->get_size() == 1) {
        int child_id = old_root_node->remove_and_return_only_child();
        auto child = fetch_node(child_id);
        child->page_hdr->parent = IX_NO_PAGE;
        file_hdr_->root_page_ = child_id;
        release_node_handle(*old_root_node);
        buffer_pool_manager_->unpin_page(child->get_page_id(), true);
        // delete child;
        return true;
    }
    return false;
}

/**
 * @brief 重新分配node和兄弟结点neighbor_node的键值对
 * Redistribute key & value pairs from one page to its sibling page. If index == 0, move sibling page's first key
 * & value pair into end of input "node", otherwise move sibling page's last key & value pair into head of input "node".
 *
 * @param neighbor_node sibling page of input "node"
 * @param node input from method coalesceOrRedistribute()
 * @param parent the parent of "node" and "neighbor_node"
 * @param index node在parent中的rid_idx
 * @note node是之前刚被删除过一个key的结点
 * index<0，则neighbor是node后继结点，表示：node(left)      neighbor(right)
 * index>0，则neighbor是node前驱结点，表示：neighbor(left)  node(right)
 * 注意更新parent结点的相关kv对
 */
void IxIndexHandle::redistribute(IxNodeHandle* neighbor_node,
                                 IxNodeHandle* node, IxNodeHandle* parent,
                                 int index) {
    // Todo:
    // 1. 通过index判断neighbor_node是否为node的前驱结点
    // 2. 从neighbor_node中移动一个键值对到node结点中
    // 3. 更新父节点中的相关信息，并且修改移动键值对对应孩字结点的父结点信息（maintain_child函数）
    // 注意：neighbor_node的位置不同，需要移动的键值对不同，需要分类讨论
    int sum = neighbor_node->get_size() + node->get_size();
    int l = sum / 2;
    auto lt = neighbor_node;
    auto rt = node;
    if (index < 0)
        std::swap(lt, rt);
    if (lt->get_size() < l) {
        // right -> left
        auto kp = rt->get_key(0);
        auto rp = rt->get_rid(0);
        int pos = lt->get_size(), cnt = l - pos;
        lt->insert_pairs(pos, kp, rp, cnt);
        rt->erase_pairs(0, cnt);
        for (int i = pos; i < pos + cnt; i++) {
            maintain_child(lt, i);
        }
    } else if (lt->get_size() > l) {
        // left -> right
        auto kp = lt->get_key(l);
        auto rp = lt->get_rid(l);
        int pos = lt->get_size(), cnt = pos - l;
        rt->insert_pairs(0, kp, rp, cnt);
        lt->erase_pairs(l, cnt);
        for (int i = 0; i < cnt; i++) {
            maintain_child(rt, i);
        }
    }
    maintain_parent(rt);
}

/**
 * @brief 合并(Coalesce)函数是将node和其直接前驱进行合并，也就是和它左边的neighbor_node进行合并；
 * 假设node一定在右边。如果上层传入的index=0，说明node在左边，那么交换node和neighbor_node，保证node在右边；合并到左结点，实际上就是删除了右结点；
 * Move all the key & value pairs from one page to its sibling page, and notify buffer pool manager to delete this page.
 * Parent page must be adjusted to take info of deletion into account. Remember to deal with coalesce or redistribute
 * recursively if necessary.
 *
 * @param neighbor_node sibling page of input "node" (neighbor_node是node的前结点)
 * @param node input from method coalesceOrRedistribute() (node结点是需要被删除的)
 * @param parent parent page of input "node"
 * @param index node在parent中的rid_idx
 * @return true means parent node should be deleted, false means no deletion happend
 * @note Assume that *neighbor_node is the left sibling of *node (neighbor -> node)
 * index<0，则neighbor是node后继结点，表示：node(left)      neighbor(right)
 * index>0，则neighbor是node前驱结点，表示：neighbor(left)  node(right)
 */
bool IxIndexHandle::coalesce(IxNodeHandle** neighbor_node, IxNodeHandle** node,
                             IxNodeHandle** parent, int index,
                             Transaction* transaction, bool* root_is_latched) {
    // Todo:
    // 1. 用index判断neighbor_node是否为node的前驱结点，若不是则交换两个结点，让neighbor_node作为左结点，node作为右结点
    // 2. 把node结点的键值对移动到neighbor_node中，并更新node结点孩子结点的父节点信息（调用maintain_child函数）
    // 3. 释放和删除node结点，并删除parent中node结点的信息，返回parent是否需要被删除
    // 提示：如果是叶子结点且为最右叶子结点，需要更新file_hdr_.last_leaf
    auto lt = *neighbor_node;
    auto rt = *node;
    if (index < 0)
        std::swap(lt, rt);
    int cnt = rt->get_size(), pos = lt->get_size();
    lt->insert_pairs(pos, rt->get_key(0), rt->get_rid(0), cnt);
    for (int i = pos; i < pos + cnt; i++) {
        maintain_child(lt, i);
    }
    (*parent)->remove(rt->get_key(0));
    if (rt->is_leaf_page() && file_hdr_->last_leaf_ == rt->get_page_no()) {
        file_hdr_->last_leaf_ = lt->get_page_no();
        //更新表头
    }
    buffer_pool_manager_->unpin_page(rt->get_page_id(), true);
    if (rt->is_leaf_page())
        erase_leaf(rt);
    release_node_handle(*rt);
    free(rt);
    return (*parent)->get_size() < (*parent)->get_min_size();
}

/**
 * @brief 这里把iid转换成了rid，即iid的slot_no作为node的rid_idx(key_idx)
 * node其实就是把slot_no作为键值对数组的下标
 * 换而言之，每个iid对应的索引槽存了一对(key,rid)，指向了(要建立索引的属性首地址,插入/删除记录的位置)
 *
 * @param iid
 * @return Rid
 * @note iid和rid存的不是一个东西，rid是上层传过来的记录位置，iid是索引内部生成的索引槽位置
 */
Rid IxIndexHandle::get_rid(const Iid& iid) const {
    IxNodeHandle* node = fetch_node(iid.page_no);
    if (iid.slot_no >= node->get_size()) {
        throw IndexEntryNotFoundError();
    }
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);  // unpin it!
    auto rid = *node->get_rid(iid.slot_no);
    // delete node;
    return rid;
}

/**
 * @brief FindLeafPage + lower_bound
 *
 * @param key
 * @return Iid
 * @note 上层传入的key本来是int类型，通过(const char *)&key进行了转换
 * 可用*(int *)key转换回去
 */
Iid IxIndexHandle::lower_bound(const char* key) {
    IxNodeHandle* node = find_leaf_page(key, Operation::FIND, nullptr).first;
    int key_idx = node->lower_bound(key);

    Iid iid = {.page_no = node->get_page_no(), .slot_no = key_idx};
    if (key_idx == node->get_size()) {
        // 说明该叶子节点不存在满足条件的值，需要找下一个叶子节点
        if (node->get_page_id().page_no == file_hdr_->last_leaf_) {
            //说明是最后一个叶子了
            iid = leaf_end();
        } else {
            //直接取下一个叶子的第一个
            iid = {.page_no = node->get_next_leaf(), .slot_no = 0};
        }
    }

    // unpin leaf node
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);
    return iid;
}

/**
 * @brief FindLeafPage + upper_bound
 *
 * @param key
 * @return Iid
 */
Iid IxIndexHandle::upper_bound(const char* key) {
    IxNodeHandle* node = find_leaf_page(key, Operation::FIND, nullptr).first;
    int key_idx = node->upper_bound(key);

    Iid iid = {.page_no = node->get_page_no(), .slot_no = key_idx};
    if (key_idx == node->get_size()) {
        // 说明该叶子节点不存在满足条件的值，需要找下一个叶子节点
        if (node->get_page_id().page_no == file_hdr_->last_leaf_) {
            //说明是最后一个叶子了
            iid = leaf_end();
        } else {
            //直接取下一个叶子的第一个
            iid = {.page_no = node->get_next_leaf(), .slot_no = 0};
        }
    }

    // unpin leaf node
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);
    return iid;
}

/**
 * @brief 指向最后一个叶子的最后一个结点的后一个
 * 用处在于可以作为IxScan的最后一个
 *
 * @return Iid
 */
Iid IxIndexHandle::leaf_end() const {
    IxNodeHandle* node = fetch_node(file_hdr_->last_leaf_);
    Iid iid = {.page_no = file_hdr_->last_leaf_, .slot_no = node->get_size()};
    buffer_pool_manager_->unpin_page(node->get_page_id(), false);  // unpin it!
    delete node;
    return iid;
}

/**
 * @brief 指向第一个叶子的第一个结点
 * 用处在于可以作为IxScan的第一个
 *
 * @return Iid
 */
Iid IxIndexHandle::leaf_begin() const {
    Iid iid = {.page_no = file_hdr_->first_leaf_, .slot_no = 0};
    return iid;
}

/**
 * @brief 获取一个指定结点
 *
 * @param page_no
 * @return IxNodeHandle*
 * @note pin the page, remember to unpin it outside!
 */
IxNodeHandle* IxIndexHandle::fetch_node(int page_no) const {
    Page* page = buffer_pool_manager_->fetch_page(PageId{fd_, page_no});
    auto* node = new IxNodeHandle(file_hdr_, page);

    return node;
}

/**
 * @brief 创建一个新结点
 *
 * @return IxNodeHandle*
 * @note pin the page, remember to unpin it outside!
 * 注意：对于Index的处理是，删除某个页面后，认为该被删除的页面是free_page
 * 而first_free_page实际上就是最新被删除的页面，初始为IX_NO_PAGE
 * 在最开始插入时，一直是create node，那么first_page_no一直没变，一直是IX_NO_PAGE
 * 与Record的处理不同，Record将未插入满的记录页认为是free_page
 */
IxNodeHandle* IxIndexHandle::create_node() {
    IxNodeHandle* node;
    file_hdr_->num_pages_++;

    PageId new_page_id = {.fd = fd_, .page_no = INVALID_PAGE_ID};
    // 从3开始分配page_no，第一次分配之后，new_page_id.page_no=3，file_hdr_.num_pages=4
    Page* page = buffer_pool_manager_->new_page(&new_page_id);
    node = new IxNodeHandle(file_hdr_, page);
    return node;
}

/**
 * @brief 从node开始更新其父节点的第一个key，一直向上更新直到根节点
 *
 * @param node
 */
void IxIndexHandle::maintain_parent(IxNodeHandle* node) {
    IxNodeHandle* curr = node;
    while (curr->get_parent_page_no() != IX_NO_PAGE) {
        // Load its parent
        IxNodeHandle* parent = fetch_node(curr->get_parent_page_no());
        int rank = parent->find_child(curr);
        char* parent_key = parent->get_key(rank);
        char* child_first_key = curr->get_key(0);
        if (memcmp(parent_key, child_first_key, file_hdr_->col_tot_len_) == 0) {
            assert(
                buffer_pool_manager_->unpin_page(parent->get_page_id(), true));
            break;
        }
        memcpy(parent_key, child_first_key,
               file_hdr_->col_tot_len_);  // 修改了parent node
        // delete curr;
        curr = parent;

        assert(buffer_pool_manager_->unpin_page(parent->get_page_id(), true));
    }
    // delete curr;
}

/**
 * @brief 要删除leaf之前调用此函数，更新leaf前驱结点的next指针和后继结点的prev指针
 *
 * @param leaf 要删除的leaf
 */
void IxIndexHandle::erase_leaf(IxNodeHandle* leaf) {
    assert(leaf->is_leaf_page());

    IxNodeHandle* prev = fetch_node(leaf->get_prev_leaf());
    prev->set_next_leaf(leaf->get_next_leaf());
    buffer_pool_manager_->unpin_page(prev->get_page_id(), true);

    IxNodeHandle* next = fetch_node(leaf->get_next_leaf());
    next->set_prev_leaf(leaf->get_prev_leaf());  // 注意此处是SetPrevLeaf()
    buffer_pool_manager_->unpin_page(next->get_page_id(), true);

    delete prev;
    delete next;
}

/**
 * @brief 删除node时，更新file_hdr_.num_pages
 *
 * @param node
 */
void IxIndexHandle::release_node_handle(IxNodeHandle& node) {
    file_hdr_->num_pages_--;
}

/**
 * @brief 将node的第child_idx个孩子结点的父节点置为node
 */
void IxIndexHandle::maintain_child(IxNodeHandle* node, int child_idx) {
    if (!node->is_leaf_page()) {
        //  Current node is inner node, load its child and set its parent to current node
        int child_page_no = node->value_at(child_idx);
        IxNodeHandle* child = fetch_node(child_page_no);
        child->set_parent_page_no(node->get_page_no());
        buffer_pool_manager_->unpin_page(child->get_page_id(), true);
        delete child;
    }
}