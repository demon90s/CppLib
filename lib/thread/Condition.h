#pragma once

#include <pthread.h>
#include "common/NoCopy.h"

// 封装 posix 条件变量

class Condition {
public:
    Condition();
    ~Condition();

    void Wait();
    bool TimeWait(unsigned long ms);
    void Signal();

private:
    NoCopy nocopy_;

    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};
