#pragma once

#include <pthread.h>
#include "common/NoCopy.h"

// 封装 posix 线程互斥琐

class Mutex {
public:
    Mutex();
    ~Mutex();

    bool Lock();
    bool UnLock();

private:
    NoCopy nocopy_;
    pthread_mutex_t mutex_;
};