#include "MemoryPool.h"

MemoryPool::MemoryPool(size_t block_sz, int increase_count) : block_sz_(block_sz), increase_count_(increase_count)
{

}

MemoryPool::~MemoryPool()
{
    for (void *p : alloc_mem_list_)
        free(p);
}

void *MemoryPool::Alloc()
{
    if (unused_mem_list_.size() <= 0) {
        this->Increase();
    }

    void *ret = unused_mem_list_.back();
    unused_mem_list_.pop_back();
    return ret;
}

void MemoryPool::Free(void *mem)
{
    unused_mem_list_.push_back(mem);
}

void MemoryPool::Increase()
{
    void *block_head = malloc(block_sz_ * increase_count_);
    alloc_mem_list_.push_back(block_head);

    for (int i = 0; i < increase_count_; i++) {
        unused_mem_list_.push_back((char*)block_head + block_sz_);
    }
}
