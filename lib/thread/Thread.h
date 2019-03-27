#pragma once

#include <pthread.h>
#include "common/NoCopy.h"

// 封装 posix 线程

class Thread {
public:
    Thread();
    ~Thread();

    typedef void* (*ThreadFunc)(void*);
    bool Run(ThreadFunc f, void *param);

    bool Join();
    bool Detach();
    bool Cancel();

private:
    NoCopy nocopy_;
    pthread_t thread_;
};
