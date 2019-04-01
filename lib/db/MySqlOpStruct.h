#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "common/Node.h"
#include "common/string_functions.h"

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

struct _MySqlFindRes {
    typedef std::unordered_map<std::string, size_t> IndexMap;

    struct Row {
        Row(std::vector<Node> *meta_row, IndexMap *index_map) : meta_row_(meta_row), index_map_(index_map) {}
        void SetNode(size_t index, const char *v) {
            if (index >= 0 && index < meta_row_->size()) {
                row_.resize(meta_row_->size());

                row_[index].SetType((*meta_row_)[index].GetType());
                row_[index].SetValue(v);
            }
        }

        const Node& GetNode(const std::string &field_name) {
            auto p = index_map_->find(field_name);
            if (p == index_map_->end())
                throw std::runtime_error(StringFormat("invalid field name: %s", field_name.c_str()));
            return row_[p->second];
        }

        std::vector<Node> row_;
        const std::vector<Node> *meta_row_;
        const IndexMap *index_map_;
    };

    IndexMap field_to_index_map; 
    std::vector<Row> res_;

    std::vector<Node> meta_row_;        // TODO 是否可以用指针
};

typedef std::shared_ptr<_MySqlFindRes> MySqlFindRes;
