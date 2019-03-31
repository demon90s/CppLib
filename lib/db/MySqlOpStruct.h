#pragma once

#include <string>
#include <vector>
#include <unordered_map>
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

struct MySqlFindRes {
    struct Row {
        Row(std::vector<Node> *meta_row) : meta_row_(meta_row) {}
        void SetNode(size_t index, const char *v) {
            if (index >= 0 && index < meta_row_->size()) {
                row_.resize(meta_row_->size());

                row_[index].SetType((*meta_row_)[index].GetType());
                row_[index].SetValue(v);
            }
        }
        std::vector<Node> row_;
        const std::vector<Node> *meta_row_;
    };

    std::unordered_map<std::string, size_t> field_to_index_map; 
    std::vector<Row> res_;

    std::vector<Node> meta_row_;        // TODO 是否可以用指针
};
