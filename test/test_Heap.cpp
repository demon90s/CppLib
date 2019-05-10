#include "test_Heap.h"
#include "common/Heap.h"
#include <iostream>

void test_Heap()
{
    Heap<int> ih;
    ih.Push(3);
    ih.Push(6);
    ih.Push(1);
    ih.Push(10);
    ih.Push(5);
    ih.Push(1);

    int i;
    while (ih.Front(&i)) {
        std::cout << i << std::endl;
        ih.PopFront();
    }
}