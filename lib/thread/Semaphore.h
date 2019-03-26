#pragma once

#include <semaphore.h>
#include "common/NoCopy.h"

// 封装 posix 线程信号量

class Semaphore {
public:
    Semaphore(int init_count);
    ~Semaphore();
    
    void Wait();
    bool TimeWait(unsigned long ms);
    void Post();

private:
    NoCopy nocopy_;
    sem_t sem_;
};
