#include "Semaphore.h"
#include <time.h>

Semaphore::Semaphore()
{
    sem_init(&sem_, 0, 0);
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
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += ms / 1000;
    ts.tv_nsec += (ms % 1000) * 1000 * 1000; // 1 毫秒 = 1,000,000纳秒

    const long BILLION = 1000000000;
    if (ts.tv_nsec >= BILLION)
    {
        ++ts.tv_sec;
        ts.tv_nsec %= BILLION;
    }

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
