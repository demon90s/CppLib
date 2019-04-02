#pragma once

#include <list>
#include <cstdlib>
#include "common/NoCopy.h"

// 非线程安全的内存池, 需要用户主动重载 operator new 和 operator delete 运算符 (可用宏代替)
// 不做内存收缩工作

class MemoryPool {
public:
    MemoryPool(size_t block_sz, int increase_count = 64);
    ~MemoryPool();

    void *Alloc();
    void Free(void *mem);

private:
    void Increase();

    NoCopy nocopy_;

    std::list<void*> unused_mem_list_;
    std::list<void*> alloc_mem_list_;

    size_t block_sz_;
    int increase_count_;        // 每次增长这么多
};

#define DEF_NEW_DELETE()\
    void *operator new(size_t c);\
    void operator delete(void *m)

#define DEC_NEW_DELETE(classname)\
    namespace classname##_MemPool {\
        MemoryPool mempool(sizeof(classname));\
    }\
    void* classname::operator new(size_t c)\
    {\
        return classname##_MemPool::mempool.Alloc();\
    }\
    void classname::operator delete(void *m)\
    {\
        classname##_MemPool::mempool.Free(m);\
    }