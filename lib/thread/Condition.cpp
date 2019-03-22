#include "Condition.h"
#include "common/clock_functions.h"

Condition::Condition()
{
    pthread_cond_init(&cond_, NULL);
    pthread_mutex_init(&mutex_, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&cond_);
    pthread_mutex_destroy(&mutex_);
}

void Condition::Wait()
{
    pthread_mutex_lock(&mutex_);
    pthread_cond_wait(&cond_, &mutex_);
    pthread_mutex_unlock(&mutex_);
}

bool Condition::TimeWait(unsigned long ms)
{
    struct timespec ts = GetTimeSpec(ms);

    return pthread_cond_timedwait(&cond_, &mutex_, &ts) == 0;
}

void Condition::Signal()
{
    pthread_cond_signal(&cond_);
}