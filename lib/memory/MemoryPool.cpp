#include "MemoryPool.h"
#include "MemoryLogger.h"
#include <cstring>

MemoryPool::MemoryPool(size_t block_sz, int increase_count) : block_sz_(block_sz), increase_count_(increase_count),
    used_blocks_(0), total_blocks(0)
{
    memset(name_, 0, sizeof(name_));
}

MemoryPool::~MemoryPool()
{
    for (void *p : alloc_mem_list_)
        free(p);
}

void *MemoryPool::Alloc()
{
    if (unused_block_list_.size() <= 0) {
        this->Increase();
    }

    void *ret = unused_block_list_.back();
    unused_block_list_.pop_back();
    ++used_blocks_;
    total_blocks = static_cast<int>(alloc_mem_list_.size() * increase_count_);
    return ret;
}

void MemoryPool::Free(void *mem)
{
    unused_block_list_.push_back(mem);
    --used_blocks_;
}

void MemoryPool::Increase()
{
    void* const block_head = malloc(block_sz_ * increase_count_);
    alloc_mem_list_.push_back(block_head);

    for (int i = 0; i < increase_count_; i++) {
        unused_block_list_.push_back((char*)block_head + block_sz_ * i);
    }
}

void MemoryPool::SetName(const char *myname)
{
    strncpy(name_, myname, sizeof(name_) - 1);
    name_[sizeof(name_) - 1] = '\0';
}