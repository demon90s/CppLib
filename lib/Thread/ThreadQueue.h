#pragma once

#include <cstdlib>
#include "common/NoCopy.h"
#include "Mutex.h"
#include "Semaphore.h"

// 线程安全的生产/消费队列, 队列元素需要支持拷贝操作
template<typename T>
class ThreadQueue
{
public:
    ThreadQueue(int init_size) : queue_len_(init_size + 1), queue_(nullptr), read_index_(0), write_index_(0), read_sem_(0), write_sem_(init_size) {
        queue_ = (T*)malloc(sizeof(T) * queue_len_);
    }

    ~ThreadQueue() {
        free(queue_);
    }

    void Push(const T &v) {
        write_sem_.Wait();  
        this->DoPush(v);
        read_sem_.Post();
    }

    bool TryPush(const T &v, unsigned long timeout) {
        if (!write_sem_.TimeWait(timeout))
            return false; // 超时则失败
        this->DoPush(v);
        read_sem_.Post();
        return true;
    }

    void Pop(T *v) {
        read_sem_.Wait();
        this->DoPop(v);
        write_sem_.Post();
    }

    bool TryPop(T *v, unsigned long timeout) {
        if (!read_sem_.TimeWait(timeout))
            return false; // 超时则失败
        this->DoPop(v);
        write_sem_.Post();
        return true;
    }

private:
    void DoPush(const T &v) {
        write_mutex_.Lock();
        new (&queue_[write_index_])T(v);
        ++write_index_;
        if (write_index_ >= queue_len_)
            write_index_ = 0;
        write_mutex_.UnLock();
    }

    void DoPop(T *v) {
        read_mutex_.Lock();
        *v = queue_[read_index_];
        queue_[read_index_].~T();
        ++read_index_;
        if (read_index_ >= queue_len_)
            read_index_ = 0;
        read_mutex_.UnLock();
    }

    NoCopy nocopy_;

    int queue_len_;
    T *queue_;
    int read_index_;            // 消费索引
    int write_index_;           // 生产索引
    
    Mutex read_mutex_;
    Mutex write_mutex_;
    Semaphore read_sem_;
    Semaphore write_sem_;
};
