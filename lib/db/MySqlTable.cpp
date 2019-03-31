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

        OneTable one_table;
        MYSQL_FIELD *field_ptr;
        while ((field_ptr = mysql_fetch_field(mysql_handler_->last_res_)) != nullptr) {
            MySqlField field_info;
            field_info.name_ = field_ptr->name;
            NodeType type = this->GetFieldType(field_ptr->type);
            if (type == NodeType::Invalid) {
                throw std::runtime_error(StringFormat("Easy engine not support this db field_type, table is: %s, field is: %s, type is: %d", 
                    table_name.c_str(), field_ptr->name, field_ptr->type).c_str());
            }
            field_info.node_.SetType(type);

            one_table.emplace(field_ptr->name, field_info);
        }

        tables.emplace(table_name, one_table);
    }

    tables_.swap(tables);

    this->ShowTables();

    return true;
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
        const OneTable &one_table = p.second;
        for (const auto &field_p : one_table) {
            const MySqlField &file_info = field_p.second;
            std::cout << "\t" << file_info.name_ <<  " --- " << file_info.node_.GetTypeStr() <<  std::endl;
        }
    }

    std::cout << "====ShowTables End=========\n";
}

//----------------------------------------