#include "test_ObjectArray.h"
#include "common/ObjectArray.h"
#include "common/other_macros.h"
#include <iostream>
#include <algorithm>

static void test1()
{
    ObjectArray<int> oa;

    oa.Add(42);
    oa.Add(100);
    oa.Add(88);

    std::cout << "oa.Size(): " << oa.Size() << std::endl;           // 3
    std::cout << "oa.Capacity(): " << oa.Capacity() << std::endl;

    int index = oa.Add(1000);
    std::cout << "index: " << index << " value: " << oa[index] <<  std::endl;    // 3 1000

    oa.Add(666);

    // 42 100 88 1000 666
    for (auto it = oa.begin(); it != oa.end(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    oa.Remove(index);
    //oa[index];    // throw out of range

    // 42 100 88 666
    for (auto i : oa) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    auto new_oa = oa;

    oa.Clear();
    std::cout << "oa.Size(): " << oa.Size() << std::endl;           // 0

    // 42 100 88 666
    for (auto i : new_oa) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    oa.Add(1);
    new_oa = oa;
    std::cout << "new_oa.Size(): " << oa.Size() << std::endl;       // 1
}

struct Foo {
    Foo() { std::cout << "Foo()\n"; }
    ~Foo() { std::cout << "~Foo()\n"; }
    Foo(const Foo&) { std::cout << "Foo(const Foo&)\n"; }
};
static void test2()
{
    Foo foo_tmp;
    ObjectArray<Foo> oa;

    std::cout << "test begin--------------------------------------------\n";

    std::cout << "oa.Size(): " << oa.Size() << std::endl;           // 0

    oa.Add(foo_tmp);
    std::cout << "oa.Size(): " << oa.Size() << std::endl;           // 1

    oa.Add(foo_tmp);
    std::cout << "oa.Size(): " << oa.Size() << std::endl;           // 2

    std::cout << "test end  --------------------------------------------\n";
}

static void test3()
{
    ObjectArray<Foo*> oa;

    // test empty array, OK~
    for (auto foo : oa) {
        delete foo;
    }

    for (int i = 0; i < 5; i++) {
        oa.Add(new Foo);
    }

    for (auto foo : oa) {
        delete foo;
    }
}

void test_ObjectArray()
{
    test1();
    IGNORE_NOUSED_WARNING(test1);
    
    //test2();
    IGNORE_NOUSED_WARNING(test2);
    
    //test3();
    IGNORE_NOUSED_WARNING(test3);
}