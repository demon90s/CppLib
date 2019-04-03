#include "test_Memory.h"
#include "memory/MemoryPool.h"
#include "memory/MemoryLogger.h"
#include "common/array_functions.h"
#include "common/clock_functions.h"
#include <iostream>

class Foo {
public:
    //Foo() { std::cout << "Foo()\n"; }
    //~Foo() { std::cout << "~Foo()\n"; }
    DEF_NEW_DELETE();
};

DEC_NEW_DELETE(Foo);

void test_Memory()
{
    MemoryLogger logger;

    MEMPOOL(Foo)->SetName("Foo");
    logger.Register(MEMPOOL(Foo));
    
    logger.StartLogger("./log"/*, "MemoryLog"*/);

    Foo* foo_list[10];

    // for (int t = 0; t < 100; t++) {
    //     for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
    //         foo_list[i] = new Foo;
    //     }

    //     for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
    //         delete foo_list[i];
    //     }
    // }

    for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
        foo_list[i] = new Foo;
        Sleep(1000);
    }

    for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
        delete foo_list[i];
        Sleep(1000);
    }
}

