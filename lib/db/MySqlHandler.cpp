#include "MySqlHandler.h"
#include "common/string_functions.h"
#include <cstring>

MySqlHandler::MySqlHandler() : mysql_(nullptr), last_res_(nullptr), table_(this)
{
    mysql_ = new MYSQL;
    mysql_init(mysql_);
}

MySqlHandler::~MySqlHandler()
{
    if (last_res_) {
        mysql_free_result(last_res_);
        last_res_ = nullptr;
    }

    mysql_close(mysql_);
    delete mysql_;
}

bool MySqlHandler::Connect(const char* host, const char* user_name, const char* passwd, const char* db)
{
    const char *sock_name = sock_name_.empty() ? 0 : sock_name_.c_str();

    if (mysql_real_connect(mysql_, host, user_name, passwd, db, 0, sock_name, 0) == nullptr) {
        return false;
    }

    if (!table_.InitTables()) {
        return false;
    }

    return true;
}

bool MySqlHandler::Insert(const MySqlOpStruct &op)
{
    // INSERT INTO tables(colomns)VALUES(values)
    std::string sql = "INSERT INTO " + op.table_name_;

    sql += '(';
    // columns
    for (const MySqlField& field : op.op_field_) {
        sql += field.name_;
        sql += ',';
    }
    sql.resize(sql.length() - 1); // remove last ,
    sql += ')';

    sql += "VALUES(";
    // values
    for (const MySqlField& field : op.op_field_) {
        if (field.node_.GetType() == NodeType::String) {
            sql += "'";
            sql += field.node_.GetValue();
            sql += "'";
        }
        else {
            sql += field.node_.GetValue();
        }
        sql += ',';
    }
    sql.resize(sql.length() - 1); // remove last ,
    sql += ')';

    return this->Query(sql);
}

bool MySqlHandler::Remove(const MySqlOpStruct &op)
{
    // DELETE FROM table WHERE conditions
    std::string sql = "DELETE FROM " + op.table_name_;

    if (op.op_field_.size() > 0) {
        sql += " WHERE ";

        for (size_t i = 0; i < op.op_field_.size(); i++) {
            const MySqlField &field = op.op_field_[i];
            std::string sub_condition = field.name_;
            switch (field.cmp_type_)
            {
                case MySqlCmpType::Equal:
                    if (field.node_.GetType() == NodeType::String) {
                        sub_condition += " LIKE ";
                    }
                    else {
                        sub_condition += '=';
                    }
                    break;
                default:
                    break;
            }
            if (field.node_.GetType() == NodeType::String) {
                sub_condition += "'";
                sub_condition += field.node_.GetValue();
                sub_condition += "'";
            }
            else {
                sub_condition += field.node_.GetValue();
            }

            if (i < op.op_field_.size() - 1) {
                switch (op.cmp_logic)
                {
                    case MySqlCmpLogic::And:
                        sub_condition += " AND";
                        break;
                    case MySqlCmpLogic::Or:
                        sub_condition += " OR";
                        break;
                    default:
                        break;
                }
            }

            sql += sub_condition;
        }
    }

    return this->Query(sql);
}

bool MySqlHandler::Update(const MySqlOpStruct &set_op, const MySqlOpStruct &cmp_op)
{
    if (set_op.op_field_.size() <= 0)
        throw std::runtime_error("no db field to update");

    // UPDATE table SET field=value,... WHERE conditions
    std::string sql = "UPDATE " + set_op.table_name_ + " SET ";

    for (size_t i = 0; i < set_op.op_field_.size(); i++) {
        const MySqlField &field = set_op.op_field_[i];
        sql += field.name_;
        sql += '=';
        if (field.node_.GetType() == NodeType::String) {
            sql += "'";
            sql += field.node_.GetValue();
            sql += "'";
        }
        else {
            sql += field.node_.GetValue();
        }

        sql += ',';
    }
    sql.resize(sql.length() - 1); // remove last ,

    if (cmp_op.op_field_.size() > 0) {
        sql += " WHERE ";

        for (size_t i = 0; i < cmp_op.op_field_.size(); i++) {
            const MySqlField &field = cmp_op.op_field_[i];
            std::string sub_condition = field.name_;
            switch (field.cmp_type_)
            {
                case MySqlCmpType::Equal:
                    if (field.node_.GetType() == NodeType::String) {
                        sub_condition += " LIKE ";
                    }
                    else {
                        sub_condition += '=';
                    }
                    break;
                default:
                    break;
            }
            if (field.node_.GetType() == NodeType::String) {
                sub_condition += "'";
                sub_condition += field.node_.GetValue();
                sub_condition += "'";
            }
            else {
                sub_condition += field.node_.GetValue();
            }

            if (i < cmp_op.op_field_.size() - 1) {
                switch (cmp_op.cmp_logic)
                {
                    case MySqlCmpLogic::And:
                        sub_condition += " AND";
                        break;
                    case MySqlCmpLogic::Or:
                        sub_condition += " OR";
                        break;
                    default:
                        break;
                }
            }

            sql += sub_condition;
        }
    }

    return this->Query(sql);
}

std::string MySqlHandler::ErrorDesc()
{
    return StringFormat("(%u) %s", mysql_errno(mysql_), mysql_error(mysql_));
}

bool MySqlHandler::Query(const std::string &sql, bool store_result)
{
    if (last_res_) {
        mysql_free_result(last_res_);
        last_res_ = nullptr;
    }

    char buffer[2048] = {};
    if (sql.length() >= sizeof(buffer) - 1) {
        throw std::runtime_error("sql length is too long");
    }
    memcpy(buffer, sql.c_str(), sql.length());

    int ret = mysql_real_query(mysql_, buffer, strlen(buffer) + 1);
    if (ret != 0)
        return false;

    if (store_result) {
        last_res_ = mysql_store_result(mysql_);
        if (!last_res_)
            return false;
    }

    return true;
}
