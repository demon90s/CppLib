#pragma once

#include "common/NoCopy.h"
#include "mysql/include/mysql.h"
#include "MySqlTable.h"
#include "MySqlOpStruct.h"
#include <string>

class MySqlHandler {
    friend class MySqlTable;
public:
    MySqlHandler();
    ~MySqlHandler();

    void SetSockName(const char* sock_name) { sock_name_ = sock_name; }

    bool Connect(const char* host, const char* user_name, const char* passwd, const char* db);
    bool Insert(const MySqlOpStruct &op);
    bool Remove(const MySqlOpStruct &op);
    bool Update(const MySqlOpStruct &set_op, const MySqlOpStruct &cmp_op);
    MySqlFindRes Find(const MySqlOpStruct &cmp_op);

    std::string ErrorDesc();

private:
    bool Query(const std::string &sql, bool store_result = false);
    std::string MakeConditions(const MySqlOpStruct cmp_op);

    NoCopy nocopy_;

    MYSQL *mysql_;
    std::string sock_name_;
    MYSQL_RES *last_res_;
    
    MySqlTable table_;
};
