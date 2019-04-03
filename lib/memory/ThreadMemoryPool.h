#pragma once

#include "MemoryPool.h"
#include "thread/Mutex.h"

// 线程安全的内存池
class ThreadMemoryPool {
public:
    ThreadMemoryPool(size_t block_sz, int increase_count = 64);

    void *Alloc();
    void Free(void *mem);

private:
    MemoryPool memory_pool_;
    Mutex mutex_;
};

#define DEC_THREAD_NEW_DELETE(classname)\
    namespace classname##_MemPool {\
        ThreadMemoryPool mempool(sizeof(classname));\
    }\
    void* classname::operator new(size_t c)\
    {\
        return classname##_MemPool::mempool.Alloc();\
    }\
    void classname::operator delete(void *m)\
    {\
        classname##_MemPool::mempool.Free(m);\
    }
