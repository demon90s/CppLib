#include "MySqlTable.h"
#include "MySqlHandler.h"
#include "common/string_functions.h"
#include <unordered_set>
#include <iostream>
#include <stdexcept>

MySqlTable::MySqlTable(MySqlHandler *mysql_handler) : mysql_handler_(mysql_handler)
{

}

bool MySqlTable::InitTables()
{
    AllTable tables;
    std::unordered_set<std::string> tables_name;

    // collect all tables name
    if (!mysql_handler_->Query("SHOW TABLES", true))
        return false;
    MYSQL_ROW sqlrow;
    while ((sqlrow = mysql_fetch_row(mysql_handler_->last_res_))) {
        tables_name.insert(sqlrow[0]);
    }

    // collect all fields info
    
    for (const std::string &table_name : tables_name) {
        if (!mysql_handler_->Query(StringFormat("SELECT * FROM %s", table_name.c_str()), true))
            return false;

        MetaTable meta_table;
        MYSQL_FIELD *field_ptr;

        int field_index = 0;
        MySqlFindRes &init_findres = init_findreses_[table_name];
        init_findres = std::make_shared<_MySqlFindRes>();
        init_findres->meta_row_.clear();

        while ((field_ptr = mysql_fetch_field(mysql_handler_->last_res_)) != nullptr) {
            MySqlField field_info;
            field_info.name_ = field_ptr->name;
            NodeType type = this->GetFieldType(field_ptr->type);
            if (type == NodeType::Invalid) {
                throw std::runtime_error(StringFormat("Easy engine not support this db field_type, table is: %s, field is: %s, type is: %d", 
                    table_name.c_str(), field_ptr->name, field_ptr->type).c_str());
            }
            field_info.node_.SetType(type);

            meta_table.push_back(field_info);

            init_findres->field_to_index_map[field_info.name_] = field_index++;
            init_findres->meta_row_.push_back(field_info.node_);
        }

        tables.emplace(table_name, meta_table);
    }

    tables_.swap(tables);

    this->ShowTables();

    return true;
}

MySqlFindRes MySqlTable::InitFindRes(const std::string &table)
{
    auto it = init_findreses_.find(table);
    if (it == init_findreses_.end())
        return MySqlFindRes();

    return it->second;
}

NodeType MySqlTable::GetFieldType(int type)
{
    switch (type) {
        case MYSQL_TYPE_TINY:               // tinyint
            return NodeType::Char;
        case MYSQL_TYPE_SHORT:              // smallint
            return NodeType::Short;
        case MYSQL_TYPE_LONG:               // int
            return NodeType::Int;        
        case MYSQL_TYPE_LONGLONG:           // bigint
            return NodeType::LongLong;   
        case MYSQL_TYPE_VAR_STRING:         // varbinary
        case MYSQL_TYPE_BLOB:               // 实际是 mediumblob
            return NodeType::String;
        default:
            return NodeType::Invalid;
    }
}

//----------------------------------------
// test

void MySqlTable::ShowTables() const
{
    std::cout << "====ShowTables Begin======= total " << tables_.size() << " tables\n";
    for (const auto &p: tables_) {
        std::cout << p.first << std::endl;
        const MetaTable &meta_table = p.second;
        for (const MySqlField &field : meta_table) {
            std::cout << "\t" << field.name_ <<  " --- " << field.node_.GetTypeStr() <<  std::endl;
        }
    }

    std::cout << "====ShowTables End=========\n";
}

//----------------------------------------