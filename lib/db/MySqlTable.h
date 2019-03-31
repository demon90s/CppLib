#pragma once

#include <string>
#include <unordered_map>
#include "MySqlOpStruct.h"

class MySqlHandler;

class MySqlTable {
public:
    MySqlTable(MySqlHandler *mysql_handler);

    bool InitTables();
    MySqlFindRes InitFindRes(const std::string &table);

private:
    NodeType GetFieldType(int type);

    //-----------------------
    // test
    void ShowTables() const;
    //-----------------------

    MySqlHandler *mysql_handler_;

    typedef std::vector<MySqlField> MetaTable;
    typedef std::unordered_map<std::string, MetaTable> AllTable;
    AllTable tables_;

    std::unordered_map<std::string, MySqlFindRes> init_findreses_;
};
