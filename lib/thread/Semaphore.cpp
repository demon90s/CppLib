#include "Semaphore.h"
#include "common/clock_functions.h"
#include <time.h>

Semaphore::Semaphore(int init_count)
{
    sem_init(&sem_, 0, init_count);
}

Semaphore::~Semaphore()
{
    sem_destroy(&sem_);
}

void Semaphore::Wait()
{
    sem_wait(&sem_);
}

bool Semaphore::TimeWait(unsigned long ms)
{
    struct timespec ts = GetTimeSpec(ms);

    if (sem_timedwait(&sem_, &ts) == 0) {
        return true;
    }
    else {
        return false;
    }
}

void Semaphore::Post()
{
    sem_post(&sem_);
}
