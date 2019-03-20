#include "test_XmlHandler.h"
#include "config/XmlHandler.h"
#include "common/other_macros.h"
#include <iostream>

static void test1()
{
    XmlHandler xh;
    /*
    {
        const char* name = xh.GetValueStr("config/name");
        if (!name) {
            std::cerr << xh.ErrorDesc() << std::endl;
            return;
        }
    }
    */

    if (!xh.Parse("../config/test.xml")) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    /*
    if (!xh.GetValueStr("config")) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }
    */

    const char* name = xh.GetValueStr("config/name");
    if (name) {
        std::cout << "got name: " << name << std::endl;
    }
    else {
        std::cerr << xh.ErrorDesc() << std::endl;
    }

    int age = 0;
    if (xh.GetValue("config/age", age)) {
        std::cout << "got age: " << age << std::endl;
    }

    long long number;
    if (xh.GetValue("config/phone/number", number)) {
        std::cout << "got number: " << number << std::endl;
    }
}

static void test2()
{
    XmlHandler xh("../config/test3.xml");
    if (!xh.Parse()) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    if (!xh.SetValueStr("config/test/data", "qqqq")) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    const char *bbb = "bbbbbbbb";
    if (!xh.SetValue("config/test/bbbb", bbb)) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    int n = 42;
    if (!xh.SetValue("config/test/far", n)) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    if (!xh.Save()) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }
}

static void test3()
{
    XmlHandler xh("../config/test3.xml");
    if (!xh.Parse()) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }

    std::string tmp = "tmp";
    if (!xh.SyncValueStr("config/test/data", tmp, tmp.c_str())) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }
    else {
        std::cout << "got data: " << tmp << std::endl;
    }

    double PI = 3.14;
    if (!xh.SyncValue("config/math/pi", PI, PI)) {
        std::cerr << xh.ErrorDesc() << std::endl;
        return;
    }
    else {
        std::cout << "got PI: " << PI << std::endl;
    }

    xh.Save();
}

void test_XmlHandler()
{
    IGNORE_NOUSED_WARNING(test1);
    //test1();

    IGNORE_NOUSED_WARNING(test2);
    //test2();

    IGNORE_NOUSED_WARNING(test3);
    test3();
}