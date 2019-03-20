#include "test_string_functions.h"
#include "common/string_functions.h"
#include <iostream>

void test_StringFormat()
{
    std::string str = StringFormat("haha, %d, %s", 42, "wow");
    std::cout << str << std::endl;
}

void test_string_functions()
{
    test_StringFormat();
}