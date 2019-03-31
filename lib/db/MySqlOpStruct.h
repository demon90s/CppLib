#pragma once

#include <string>
#include <vector>
#include "common/Node.h"

enum class MySqlCmpType {
    Equal, 
};

enum class MySqlCmpLogic {
    And, Or
};

struct MySqlField {
    std::string name_;
    Node node_;
    MySqlCmpType cmp_type_;
};

struct MySqlOpStruct {
    std::string table_name_;
    MySqlCmpLogic cmp_logic;
    std::vector<MySqlField> op_field_;
};
