#include "Mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&mutex_, NULL);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mutex_);
}

bool Mutex::Lock()
{
    return pthread_mutex_lock(&mutex_) == 0;
}

bool Mutex::UnLock()
{
    return pthread_mutex_unlock(&mutex_) == 0;
}