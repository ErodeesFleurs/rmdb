/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include "common/config.h"
#include "log_defs.h"
#include "record/rm_defs.h"

/* 日志记录对应操作的类型 */
enum class LogType : int {
    UPDATE = 0,
    INSERT,
    DELETE,
    BEGIN,
    COMMIT,
    ABORT,
    INDEX_INSERT,
    INDEX_DELETE
};
static std::string LogTypeStr[] = {"UPDATE",       "INSERT",      "DELETE",
                                   "BEGIN",        "COMMIT",      "ABORT",
                                   "INDEX_INSERT", "INDEX_DELETE"};

class LogRecord {
   public:
    LogType log_type_;     /* 日志对应操作的类型 */
    lsn_t lsn_;            /* 当前日志的lsn */
    uint32_t log_tot_len_; /* 整个日志记录的长度 */
    txn_id_t log_tid_;     /* 创建当前日志的事务ID */
    lsn_t prev_lsn_; /* 事务创建的前一条日志记录的lsn，用于undo */

    // 把日志记录序列化到dest中
    virtual void serialize(char* dest) const {
        memcpy(dest + OFFSET_LOG_TYPE, &log_type_, sizeof(LogType));
        memcpy(dest + OFFSET_LSN, &lsn_, sizeof(lsn_t));
        memcpy(dest + OFFSET_LOG_TOT_LEN, &log_tot_len_, sizeof(uint32_t));
        memcpy(dest + OFFSET_LOG_TID, &log_tid_, sizeof(txn_id_t));
        memcpy(dest + OFFSET_PREV_LSN, &prev_lsn_, sizeof(lsn_t));
    }
    // 从src中反序列化出一条日志记录
    virtual void deserialize(const char* src) {
        log_type_ = *reinterpret_cast<const LogType*>(src);
        lsn_ = *reinterpret_cast<const lsn_t*>(src + OFFSET_LSN);
        log_tot_len_ =
            *reinterpret_cast<const uint32_t*>(src + OFFSET_LOG_TOT_LEN);
        log_tid_ = *reinterpret_cast<const txn_id_t*>(src + OFFSET_LOG_TID);
        prev_lsn_ = *reinterpret_cast<const lsn_t*>(src + OFFSET_PREV_LSN);
    }
    // used for debug
    virtual void format_print() {
        std::cout << "log type in father_function: "
                  << LogTypeStr[(int)log_type_] << "\n";
        printf("Print Log Record:\n");
        printf("log_type_: %s\n", LogTypeStr[(int)log_type_].c_str());
        printf("lsn: %d\n", lsn_);
        printf("log_tot_len: %d\n", log_tot_len_);
        printf("log_tid: %d\n", log_tid_);
        printf("prev_lsn: %d\n", prev_lsn_);
    }
};

class BeginLogRecord : public LogRecord {
   public:
    BeginLogRecord() {
        log_type_ = LogType::BEGIN;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }
    BeginLogRecord(txn_id_t txn_id) : BeginLogRecord() { log_tid_ = txn_id; }
    // 序列化Begin日志记录到dest中
    void serialize(char* dest) const override { LogRecord::serialize(dest); }
    // 从src中反序列化出一条Begin日志记录
    void deserialize(const char* src) override { LogRecord::deserialize(src); }
    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[(int)log_type_]
                  << "\n";
        LogRecord::format_print();
    }
};

/**
 * TODO: commit操作的日志记录
*/
class CommitLogRecord : public LogRecord {
   public:
    CommitLogRecord() {
        log_type_ = LogType::COMMIT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }
    CommitLogRecord(txn_id_t txn_id) : CommitLogRecord() { log_tid_ = txn_id; }
    // 序列化Commit日志记录到dest中
    void serialize(char* dest) const override { LogRecord::serialize(dest); }
    // 从src中反序列化出一条Commit日志记录
    void deserialize(const char* src) override { LogRecord::deserialize(src); }
    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[(int)log_type_]
                  << "\n";
        LogRecord::format_print();
    }
};

/**
 * TODO: abort操作的日志记录
*/
class AbortLogRecord : public LogRecord {
   public:
    AbortLogRecord() {
        log_type_ = LogType::ABORT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
    }
    AbortLogRecord(txn_id_t txn_id) : AbortLogRecord() { log_tid_ = txn_id; }
    // 序列化Abort日志记录到dest中
    void serialize(char* dest) const override { LogRecord::serialize(dest); }
    // 从src中反序列化出一条Abort日志记录
    void deserialize(const char* src) override { LogRecord::deserialize(src); }
    virtual void format_print() override {
        std::cout << "log type in son_function: " << LogTypeStr[(int)log_type_]
                  << "\n";
        LogRecord::format_print();
    }
};

class InsertLogRecord : public LogRecord {
   public:
    InsertLogRecord() {
        log_type_ = LogType::INSERT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }
    InsertLogRecord(txn_id_t txn_id, RmRecord& insert_value, Rid& rid,
                    std::string table_name)
        : InsertLogRecord() {
        log_tid_ = txn_id;
        insert_value_ = insert_value;
        rid_ = rid;
        log_tot_len_ += sizeof(int);
        log_tot_len_ += insert_value_.size;
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.length();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }

    // 把insert日志记录序列化到dest中
    void serialize(char* dest) const override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        memcpy(dest + offset, &insert_value_.size, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, insert_value_.data, insert_value_.size);
        offset += insert_value_.size;
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }
    // 从src中反序列化出一条Insert日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        insert_value_.Deserialize(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA + insert_value_.size + sizeof(int);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, src + offset, table_name_size_);
    }
    void format_print() override {
        printf("insert record\n");
        LogRecord::format_print();
        printf("insert_value: %s\n", insert_value_.data);
        printf("insert rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord insert_value_;   // 插入的记录
    Rid rid_;                 // 记录插入的位置
    char* table_name_;        // 插入记录的表名称
    size_t table_name_size_;  // 表名称的大小
};

/**
 * TODO: delete操作的日志记录
*/
class DeleteLogRecord : public LogRecord {
   public:
    DeleteLogRecord() {
        log_type_ = LogType::DELETE;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }
    DeleteLogRecord(txn_id_t txn_id, RmRecord& delete_value, Rid& rid,
                    std::string table_name)
        : DeleteLogRecord() {
        log_tid_ = txn_id;
        delete_value_ = delete_value;
        rid_ = rid;
        log_tot_len_ += sizeof(int);
        log_tot_len_ += delete_value_.size;
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.length();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }
    // 把delete日志记录序列化到dest中
    void serialize(char* dest) const override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        memcpy(dest + offset, &delete_value_.size, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, delete_value_.data, delete_value_.size);
        offset += delete_value_.size;
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }
    // 从src中反序列化出一条Delete日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        delete_value_.Deserialize(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA + delete_value_.size + sizeof(int);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, src + offset, table_name_size_);
    }
    void format_print() override {
        printf("delete record\n");
        LogRecord::format_print();
        printf("delete_value: %s\n", delete_value_.data);
        printf("delete rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord delete_value_;   // 删除的记录
    Rid rid_;                 // 记录删除的位置
    char* table_name_;        // 删除记录的表名称
    size_t table_name_size_;  // 表名称的大小
};

/**
 * TODO: update操作的日志记录
*/
class UpdateLogRecord : public LogRecord {
   public:
    UpdateLogRecord() {
        log_type_ = LogType::UPDATE;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        table_name_ = nullptr;
    }
    UpdateLogRecord(txn_id_t txn_id, RmRecord& before_value,
                    RmRecord& after_value, Rid& rid, std::string table_name)
        : UpdateLogRecord() {
        log_tid_ = txn_id;
        before_value_ = before_value;
        after_value_ = after_value;
        rid_ = rid;
        log_tot_len_ += sizeof(int);
        log_tot_len_ += before_value_.size;
        log_tot_len_ += sizeof(int);
        log_tot_len_ += after_value_.size;
        log_tot_len_ += sizeof(Rid);
        table_name_size_ = table_name.length();
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, table_name.c_str(), table_name_size_);
        log_tot_len_ += sizeof(size_t) + table_name_size_;
    }
    // 把update日志记录序列化到dest中
    void serialize(char* dest) const override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        memcpy(dest + offset, &before_value_.size, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, before_value_.data, before_value_.size);
        offset += before_value_.size;
        memcpy(dest + offset, &after_value_.size, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, after_value_.data, after_value_.size);
        offset += after_value_.size;
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &table_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, table_name_, table_name_size_);
    }
    // 从src中反序列化出一条Update日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        before_value_.Deserialize(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA + before_value_.size + sizeof(int);
        after_value_.Deserialize(src + offset);
        offset += after_value_.size + sizeof(int);
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        table_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        table_name_ = new char[table_name_size_];
        memcpy(table_name_, src + offset, table_name_size_);
    }
    void format_print() override {
        printf("update record\n");
        LogRecord::format_print();
        printf("before_value: %s\n", before_value_.data);
        printf("after_value: %s\n", after_value_.data);
        printf("update rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("table name: %s\n", table_name_);
    }

    RmRecord before_value_;   // 更新前的记录
    RmRecord after_value_;    // 更新后的记录
    Rid rid_;                 // 记录插入的位置
    char* table_name_;        // 插入记录的表名称
    size_t table_name_size_;  // 表名称的大小
};

class IndexInsertLogRecord : public LogRecord {
   public:
    IndexInsertLogRecord() {
        log_type_ = LogType::INDEX_INSERT;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        ix_name_ = nullptr;
    }
    IndexInsertLogRecord(txn_id_t txn_id, char* key, Rid& rid,
                         std::string ix_name, int tot_len)
        : IndexInsertLogRecord() {
        log_tid_ = txn_id;
        key_ = key;
        tot_len_ = tot_len;
        rid_ = rid;
        log_tot_len_ += sizeof(int);           // tot_len
        log_tot_len_ += tot_len;               // key
        log_tot_len_ += sizeof(Rid);           // rid
        ix_name_size_ = ix_name.length() + 1;  // ix_name
        ix_name_ = new char[ix_name_size_];
        memcpy(ix_name_, ix_name.c_str(), ix_name_size_);
        log_tot_len_ += sizeof(size_t) + ix_name_size_;  // ix_size + ix_name
    }

    // 把delete日志记录序列化到dest中
    void serialize(char* dest) const override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        //head - key_size - key - rid - ix_size - ix_name
        memcpy(dest + offset, &tot_len_, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, key_, tot_len_);
        offset += tot_len_;
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &ix_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, ix_name_, ix_name_size_);
    }
    // 从src中反序列化出一条delete日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        tot_len_ = *reinterpret_cast<const int*>(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA + sizeof(int);
        key_ = new char[tot_len_];
        memcpy(key_, src + offset, tot_len_);
        offset += tot_len_;
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        ix_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        ix_name_ = new char[ix_name_size_];
        memcpy(ix_name_, src + offset, ix_name_size_);
    }
    void format_print() override {
        printf("index insert record\n");
        LogRecord::format_print();
        printf("delete_value: %s\n", key_);
        printf("delete rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("ix name: %s\n", ix_name_);
    }

    char* key_;            // b+树插入的键值
    int tot_len_;          // key长度
    Rid rid_;              // 插入的rid
    char* ix_name_;        // index_name
    size_t ix_name_size_;  // index_name的大小
};

class IndexDeleteLogRecord : public LogRecord {
   public:
    IndexDeleteLogRecord() {
        log_type_ = LogType::INDEX_DELETE;
        lsn_ = INVALID_LSN;
        log_tot_len_ = LOG_HEADER_SIZE;
        log_tid_ = INVALID_TXN_ID;
        prev_lsn_ = INVALID_LSN;
        ix_name_ = nullptr;
    }
    IndexDeleteLogRecord(txn_id_t txn_id, char* key, Rid& rid,
                         std::string ix_name, int tot_len)
        : IndexDeleteLogRecord() {
        log_tid_ = txn_id;
        key_ = key;
        tot_len_ = tot_len;
        rid_ = rid;
        log_tot_len_ += sizeof(int);           // tot_len
        log_tot_len_ += tot_len;               // key
        log_tot_len_ += sizeof(Rid);           // rid
        ix_name_size_ = ix_name.length() + 1;  // ix_name
        ix_name_ = new char[ix_name_size_];
        memcpy(ix_name_, ix_name.c_str(), ix_name_size_);
        log_tot_len_ += sizeof(size_t) + ix_name_size_;  // ix_size + ix_name
    }

    // 把delete日志记录序列化到dest中
    void serialize(char* dest) const override {
        LogRecord::serialize(dest);
        int offset = OFFSET_LOG_DATA;
        //head - key_size - key - rid - ix_size - ix_name
        memcpy(dest + offset, &tot_len_, sizeof(int));
        offset += sizeof(int);
        memcpy(dest + offset, key_, tot_len_);
        offset += tot_len_;
        memcpy(dest + offset, &rid_, sizeof(Rid));
        offset += sizeof(Rid);
        memcpy(dest + offset, &ix_name_size_, sizeof(size_t));
        offset += sizeof(size_t);
        memcpy(dest + offset, ix_name_, ix_name_size_);
    }
    // 从src中反序列化出一条delete日志记录
    void deserialize(const char* src) override {
        LogRecord::deserialize(src);
        tot_len_ = *reinterpret_cast<const int*>(src + OFFSET_LOG_DATA);
        int offset = OFFSET_LOG_DATA + sizeof(int);
        key_ = new char[tot_len_];
        memcpy(key_, src + offset, tot_len_);
        offset += tot_len_;
        rid_ = *reinterpret_cast<const Rid*>(src + offset);
        offset += sizeof(Rid);
        ix_name_size_ = *reinterpret_cast<const size_t*>(src + offset);
        offset += sizeof(size_t);
        ix_name_ = new char[ix_name_size_];
        memcpy(ix_name_, src + offset, ix_name_size_);
    }
    void format_print() override {
        printf("index delete record\n");
        LogRecord::format_print();
        printf("delete_value: %s\n", key_);
        printf("delete rid: %d, %d\n", rid_.page_no, rid_.slot_no);
        printf("ix name: %s\n", ix_name_);
    }

    char* key_;            // b+树插入的键值
    int tot_len_;          // key长度
    Rid rid_;              // 插入的rid
    char* ix_name_;        // index_name
    size_t ix_name_size_;  // index_name的大小
};

/* 日志缓冲区，只有一个buffer，因此需要阻塞地去把日志写入缓冲区中 */

class LogBuffer {
   public:
    LogBuffer() {
        offset_ = 0;
        std::memset(buffer_, 0, sizeof(buffer_));
        log_offset_.clear();
    }

    bool is_full(int append_size) {
        if (offset_ + append_size > LOG_BUFFER_SIZE)
            return true;
        return false;
    }

    void write_log(LogRecord* log_record) {
        log_offset_.push_back(offset_);
        auto dest = std::make_unique<char[]>(log_record->log_tot_len_);
        log_record->serialize(dest.get());
        std::memcpy(buffer_ + offset_, dest.get(), log_record->log_tot_len_);
        offset_ += log_record->log_tot_len_;
    }

    void reset() { idx_ = 0; }

    void clear() {
        idx_ = 0;
        offset_ = 0;
        std::memset(buffer_, 0, sizeof(buffer_));
        log_offset_.clear();
    }

    bool has_next() const { return idx_ < (int)log_offset_.size(); }

    std::shared_ptr<LogRecord> next() {
        std::shared_ptr<LogRecord> log_record = nullptr;
        if (idx_ >= (int)log_offset_.size()) {
            return nullptr;
        }
        auto type = *reinterpret_cast<LogType*>(buffer_ + log_offset_[idx_]);

        switch (type) {
            case LogType::BEGIN: {
                log_record = std::make_shared<BeginLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
            case LogType::COMMIT: {
                log_record = std::make_shared<CommitLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
            case LogType::ABORT: {
                log_record = std::make_shared<AbortLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
            case LogType::INSERT: {
                log_record = std::make_shared<InsertLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
            case LogType::DELETE: {
                log_record = std::make_shared<DeleteLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
            case LogType::UPDATE: {
                log_record = std::make_shared<UpdateLogRecord>();
                log_record->deserialize(buffer_ + log_offset_[idx_]);
            }
        }
        idx_++;
        return log_record;
    }

    char buffer_[LOG_BUFFER_SIZE + 1];
    std::vector<int> log_offset_;  // 记录每条日志在buffer中的offset
    int offset_;                   // 写入log的offset
    int idx_ = 0;  // 用于遍历log_offset_，指示当前遍历到的log_offset_的下标
};

/* 日志管理器，负责把日志写入日志缓冲区，以及把日志缓冲区中的内容写入磁盘中 */
class LogManager {
   public:
    LogManager(DiskManager* disk_manager) { disk_manager_ = disk_manager; }

    lsn_t add_log_to_buffer(LogRecord* log_record);
    void flush_log_to_disk();

    LogBuffer* get_log_buffer() { return &log_buffer_; }

   private:
    std::atomic<lsn_t> global_lsn_{0};  // 全局lsn，递增，用于为每条记录分发lsn
    std::mutex latch_;                  // 用于对log_buffer_的互斥访问
    LogBuffer log_buffer_;  // 日志缓冲区
    lsn_t persist_lsn_;  // 记录已经持久化到磁盘中的最后一条日志的日志号
    DiskManager* disk_manager_;
};
