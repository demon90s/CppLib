#include "test_MySqlHandler.h"
#include "db/MySqlHandler.h"
#include <iostream>

void test_MySqlHandler()
{
    MySqlHandler mh;

    if (!mh.Connect("127.0.0.1", "diwen", "123456", "test")) {
        std::cerr << "MySql Connect failed: " << mh.ErrorDesc() << std::endl;
        return;
    }
    std::cout << "MySql Connect succ.\n";

    mh.BeginTrasaction();

    MySqlOpStruct op;

    // test insert
    op.table_name_ = "foo";
    op.op_field_ = {
        {"prof", 'a'},
        {"name", "liudiwen"},
        {"age", 29}
    };
    if (!mh.Insert(op)) {
        std::cerr << "MySql Insert failed: " << mh.ErrorDesc() << std::endl;
        return;
    }
    op.op_field_ = {
        {"prof", 'b'},
        {"name", "miemie"},
        {"age", 3}
    };
    if (!mh.Insert(op)) {
        std::cerr << "MySql Insert failed: " << mh.ErrorDesc() << std::endl;
        return;
    }

    // test remove
    op.cmp_logic = MySqlCmpLogic::And;
    op.op_field_ = {
        {"name", "liudiwen", MySqlCmpType::Equal}
    };
    if (!mh.Remove(op)) {
        std::cerr << "MySql Remove failed: " << mh.ErrorDesc() << std::endl;
        return;
    }

    // test update
    MySqlOpStruct set_op;
    set_op.table_name_ = "foo";
    set_op.op_field_ = {
        {"age", 14},
    };
    MySqlOpStruct cmp_op;
    cmp_op.op_field_ = {
        {"name", "miemie", MySqlCmpType::Equal},
    };

    if (!mh.Update(set_op, cmp_op)) {
        std::cerr << "MySql Update failed: " << mh.ErrorDesc() << std::endl;
        return;
    }

    // test find
    cmp_op.table_name_ = "foo";
    MySqlFindRes res = mh.Find(cmp_op);
    if (!res) {
        std::cerr << "MySql Find failed: " << mh.ErrorDesc() << std::endl;
        return;
    }
    for (size_t i = 0; i < res->res_.size(); i++) {
        printf("%zu\t", i);
        printf("prof: %c\t", res->res_[i].GetNode("prof").GetChar());
        printf("name: %s\t", res->res_[i].GetNode("name").GetString());
        printf("age: %d\t", res->res_[i].GetNode("age").GetShort());
        printf("\n");
    }

    mh.Commit();      // if not commit when trasaction begin, nothing happened
    //mh.Rollback();  // if rollback, nothing happened, check table
}
