#pragma once

#include <string>
#include <unordered_map>
#include "MySqlOpStruct.h"

class MySqlHandler;

class MySqlTable {
public:
    MySqlTable(MySqlHandler *mysql_handler);

    bool InitTables();

private:
    NodeType GetFieldType(int type);

    //-----------------------
    // test
    void ShowTables() const;
    //-----------------------

    MySqlHandler *mysql_handler_;

    typedef std::unordered_multimap<std::string, MySqlField> OneTable; // field_name -> field_info
    typedef std::unordered_map<std::string, OneTable> AllTable; // table_name -> { field_name -> field_info, ... }
    AllTable tables_;
};
