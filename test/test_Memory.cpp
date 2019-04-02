#include "test_Memory.h"
#include "memory/MemoryPool.h"
#include "common/array_functions.h"

class Foo {
public:
    DEF_NEW_DELETE();
};

DEC_NEW_DELETE(Foo);

void test_Memory()
{
    Foo* foo_list[4];

    for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
        foo_list[i] = new Foo;
    }

    for (int i = 0; i < ELEM_NUM(foo_list); ++i) {
        delete foo_list[i];
    }
}