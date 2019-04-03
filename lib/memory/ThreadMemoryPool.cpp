#include "ThreadMemoryPool.h"

ThreadMemoryPool::ThreadMemoryPool(size_t block_sz, int increase_count) :
    memory_pool_(block_sz, increase_count)
{

}

void *ThreadMemoryPool::Alloc()
{
    mutex_.Lock();
    void* ret = memory_pool_.Alloc();
    mutex_.UnLock();
    return ret;
}

void ThreadMemoryPool::Free(void *mem)
{
    mutex_.Lock();
    memory_pool_.Free(mem);
    mutex_.UnLock();
}
