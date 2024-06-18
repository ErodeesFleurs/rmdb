/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sm_manager.h"

#include <sys/stat.h>
#include <unistd.h>

#include <fstream>

#include "index/ix.h"
#include "record/rm.h"
#include "record_printer.h"

/**
 * @description: 判断是否为一个文件夹
 * @return {bool} 返回是否为一个文件夹
 * @param {string&} db_name 数据库文件名称，与文件夹同名
 */
bool SmManager::is_dir(const std::string& db_name) {
    struct stat st;
    return stat(db_name.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

/**
 * @description: 创建数据库，所有的数据库相关文件都放在数据库同名文件夹下
 * @param {string&} db_name 数据库名称
 */
void SmManager::create_db(const std::string& db_name) {
    if (is_dir(db_name)) {
        throw DatabaseExistsError(db_name);
    }
    //为数据库创建一个子目录
    std::string cmd = "mkdir " + db_name;
    if (system(cmd.c_str()) < 0) {  // 创建一个名为db_name的目录
        throw UnixError();
    }
    if (chdir(db_name.c_str()) < 0) {  // 进入名为db_name的目录
        throw UnixError();
    }
    //创建系统目录
    DbMeta *new_db = new DbMeta();
    new_db->name_ = db_name;

    // 注意，此处ofstream会在当前目录创建(如果没有此文件先创建)和打开一个名为DB_META_NAME的文件
    std::ofstream ofs(DB_META_NAME);

    // 将new_db中的信息，按照定义好的operator<<操作符，写入到ofs打开的DB_META_NAME文件中
    ofs << *new_db;  // 注意：此处重载了操作符<<

    delete new_db;

    // 创建日志文件
    disk_manager_->create_file(LOG_FILE_NAME);

    // 回到根目录
    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 删除数据库，同时需要清空相关文件以及数据库同名文件夹
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::drop_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    std::string cmd = "rm -r " + db_name;
    if (system(cmd.c_str()) < 0) {
        throw UnixError();
    }
}

/**
 * @description: 打开数据库，找到数据库对应的文件夹，并加载数据库元数据和相关文件
 * @param {string&} db_name 数据库名称，与文件夹同名
 */
void SmManager::open_db(const std::string& db_name) {
    if (!is_dir(db_name)) {
        throw DatabaseNotFoundError(db_name);
    }
    if (chdir(db_name.c_str()) < 0) {
        throw UnixError();
    }
    std::ifstream ofs(DB_META_NAME);
    ofs >> db_;
    for (auto &[tab_name, tab_info] : db_.tabs_) {
        fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
        for (const auto &index: tab_info.indexes) {
            std::string ix_name = get_ix_manager()->get_index_name(tab_name, index.cols);
            ihs_.emplace(ix_name, get_ix_manager()->open_index(tab_name, index.cols));
        }
    }
}

/**
 * @description: 把数据库相关的元数据刷入磁盘中
 */
void SmManager::flush_meta() {
    // 默认清空文件
    std::ofstream ofs(DB_META_NAME);
    ofs << db_;
}

/**
 * @description: 关闭数据库并把数据落盘
 */
void SmManager::close_db() {
    flush_meta();
    for (auto &fh: fhs_) { // 关闭所有的文件句柄
        rm_manager_->close_file(fh.second.get());
    }
    db_.name_.clear();
    db_.tabs_.clear();
    fhs_.clear();

    if (chdir("..") < 0) {
        throw UnixError();
    }
}

/**
 * @description: 显示所有的表,通过测试需要将其结果写入到output.txt,详情看题目文档
 * @param {Context*} context 
 */
void SmManager::show_tables(Context* context) {
    std::fstream outfile;
    outfile.open("output.txt", std::ios::out | std::ios::app);
    outfile << "| Tables |\n";
    RecordPrinter printer(1);
    printer.print_separator(context);
    printer.print_record({"Tables"}, context);
    printer.print_separator(context);
    for (auto &entry : db_.tabs_) {
        auto &tab = entry.second;
        printer.print_record({tab.name}, context);
        outfile << "| " << tab.name << " |\n";
    }
    printer.print_separator(context);
    outfile.close();
}

/**
 * @description: 显示表的元数据
 * @param {string&} tab_name 表名称
 * @param {Context*} context 
 */
void SmManager::desc_table(const std::string& tab_name, Context* context) {
    TabMeta &tab = db_.get_table(tab_name);

    std::vector<std::string> captions = {"Field", "Type", "Index"};
    RecordPrinter printer(captions.size());
    // Print header
    printer.print_separator(context);
    printer.print_record(captions, context);
    printer.print_separator(context);
    // Print fields
    for (auto &col : tab.cols) {
        std::vector<std::string> field_info = {col.name, coltype2str(col.type), col.index ? "YES" : "NO"};
        printer.print_record(field_info, context);
    }
    // Print footer
    printer.print_separator(context);
}

/**
 * @description: 创建表
 * @param {string&} tab_name 表的名称
 * @param {vector<ColDef>&} col_defs 表的字段
 * @param {Context*} context 
 */
void SmManager::create_table(const std::string& tab_name, const std::vector<ColDef>& col_defs, Context* context) {
    if (db_.is_table(tab_name)) {
        throw TableExistsError(tab_name);
    }
    // Create table meta
    int curr_offset = 0;
    TabMeta tab;
    tab.name = tab_name;
    for (auto &col_def : col_defs) {
        ColMeta col = {.tab_name = tab_name,
                       .name = col_def.name,
                       .type = col_def.type,
                       .len = col_def.len,
                       .offset = curr_offset,
                       .index = false};
        curr_offset += col_def.len;
        tab.cols.push_back(col);
    }
    // Create & open record file
    int record_size = curr_offset;  // record_size就是col meta所占的大小（表的元数据也是以记录的形式进行存储的）
    rm_manager_->create_file(tab_name, record_size);
    db_.tabs_[tab_name] = tab;
    // fhs_[tab_name] = rm_manager_->open_file(tab_name);
    fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));

    flush_meta();
}

/**
 * @description: 删除表
 * @param {string&} tab_name 表的名称
 * @param {Context*} context
 */
void SmManager::drop_table(const std::string& tab_name, Context* context) {
    if (!db_.is_table(tab_name)) { // 如果表不存在
        throw TableNotFoundError(tab_name);
    }
    TabMeta &tab = db_.get_table(tab_name); // 获取表的元数据
    for (const auto &index: tab.indexes) { // 删除表的索引
        std::vector<std::string> col_names;
        for (const auto &name: index.cols) {
            col_names.push_back(name.name);
        }
        drop_index(tab_name, col_names, context);
    }
    if (fhs_.count(tab_name)) { // 如果表的文件句柄存在
        rm_manager_->close_file(fhs_[tab_name].get());
        fhs_.erase(tab_name);
    }
    rm_manager_->destroy_file(tab_name);
    db_.tabs_.erase(tab_name);

    flush_meta();
}

/**
 * @description: 创建索引
 * @param {string&} tab_name 表的名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::create_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    TabMeta &tab = db_.get_table(tab_name);
    std::vector<ColMeta> cols;
    int tot_len = 0;
    for (const auto &i: col_names) {
        auto col = *tab.get_col(i);
        cols.push_back(col);
        tot_len += col.len;
    }
    ix_manager_->create_index(tab_name, cols);
    auto ix_name = ix_manager_->get_index_name(tab_name, cols);
    IndexMeta im = {tab_name, tot_len, (int) col_names.size(), cols};
    tab.indexes.push_back(im);
    ihs_.emplace(ix_name, ix_manager_->open_index(tab_name, cols));
    if(!fhs_.count(tab_name)){
        //如果没有打开表文件则打开
        fhs_.emplace(tab_name, rm_manager_->open_file(tab_name));
    }
    //将已有数据插入b+树中
    auto rfh = fhs_[tab_name].get();
    auto ih = ihs_[ix_name].get();
    auto scan_ = std::make_unique<RmScan>(rfh);
    bool is_fail = false;
    if (context != nullptr) context->lock_mgr_->lock_shared_on_table(context->txn_, rfh->GetFd());
    int idx = 0;
    while (!scan_->is_end()) {
        auto rid_ = scan_->rid();
        std::cout << "Rid: " << idx++ << " " << rid_.page_no << " " << rid_.slot_no << std::endl;
        auto rec = rfh->get_record(rid_, context);
        char *key = new char[tot_len];
        int offset = 0;
        for (auto & col : cols) {
            memcpy(key + offset, rec->data + col.offset, col.len);
            offset += col.len;
        }
        auto result = ih->insert_entry(key, rid_, context->txn_);
        if(result == INVALID_PAGE_ID){
            is_fail = true;
            break;
        }
        scan_->next();
    }
    if(is_fail){
        drop_index(tab_name, col_names, context);
        return;
    }

    flush_meta();
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<string>&} col_names 索引包含的字段名称
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<std::string>& col_names, Context* context) {
    // 检查表名
    if (!db_.is_table(tab_name)) {
        throw TableNotFoundError(tab_name);
    }
    // 存在性检查
    if (!ix_manager_->exists(tab_name, col_names)) {
        throw IndexNotFoundError(tab_name, col_names);
    }
    // 判断索引是否正确
    TabMeta &tab = db_.tabs_[tab_name];
    for(auto col_name : col_names) {
        auto col = tab.get_col(col_name);
        if(tab.is_col_to_index(col->name)){
            col->index = false;
        }
    }

    // 删除索引
    auto index_name = ix_manager_->get_index_name(tab_name, col_names);
    ix_manager_->close_index(ihs_.at(index_name).get());
    ix_manager_->destroy_index(ihs_.at(index_name).get(), tab_name, col_names);
    
    auto ix_meta = db_.get_table(tab_name).get_index_meta(col_names);
    db_.get_table(tab_name).indexes.erase(ix_meta);
    ihs_.erase(index_name);

    flush_meta();
}

/**
 * @description: 删除索引
 * @param {string&} tab_name 表名称
 * @param {vector<ColMeta>&} 索引包含的字段元数据
 * @param {Context*} context
 */
void SmManager::drop_index(const std::string& tab_name, const std::vector<ColMeta>& cols, Context* context) {
    std::vector<std::string> col_names;

    for (auto &col : cols) {
        col_names.push_back(col.name);
    }

    drop_index(tab_name, col_names, context);
}

/**
 * @description: 显示索引
 * @param {string&} tab_name 表名称
 * @param {Context*} context
 */
void SmManager::show_index(const std::string & tab_name, Context* context) {
    std::fstream outfile;
    outfile.open("output.txt", std::ios::out | std::ios::app);

    RecordPrinter printer(3);

    printer.print_separator(context);
    TabMeta &tab = db_.get_table(tab_name);
    for (const auto &i: tab.indexes) {
        std::string col;
        col += "(";
        for (const auto &icol : i.cols) {
            col += icol.name + ",";
        }
        if (col.back() == ',') col.pop_back();
        col += ")";
        std::vector<std::string> v = {tab_name, "unique", col};
        printer.print_record(v, context);
        outfile << "| " << tab_name << " | unique | " << col << " |\n";
    }
    printer.print_separator(context);

    outfile.close();
}