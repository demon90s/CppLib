#pragma once

#include <list>
#include <cstdlib>
#include "common/NoCopy.h"

// 非线程安全的内存池, 需要用户主动重载 operator new 和 operator delete 运算符 (可用宏代替)
// 不做内存收缩工作

#define MEMORY_INCREASE_COUNT 64

class MemoryPool {
public:
    MemoryPool(size_t block_sz, int increase_count = MEMORY_INCREASE_COUNT);
    ~MemoryPool();

    void *Alloc();
    void Free(void *mem);

    // 志记相关
    void SetName(const char *myname);
    const char* GetName() const { return name_; }
    int GetUsedBlockCount() const { return used_blocks_; }
    int GetTotalBlockCount() const { return total_blocks; }

private:
    void Increase();

    NoCopy nocopy_;

    std::list<void*> unused_block_list_;
    std::list<void*> alloc_mem_list_;

    size_t block_sz_;
    int increase_count_;        // 每次增长这么多

    char name_[64];
    int used_blocks_;
    int total_blocks;
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

#define MEMPOOL(classname) (&classname##_MemPool::mempool)