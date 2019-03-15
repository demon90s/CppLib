#include "Thread.h"

struct ThreadParam {
    Thread::ThreadFunc f;
    void *param;
};

static void* DoThreadFunc(void *param)
{
    ThreadParam *p = (ThreadParam*)param;

    void *ret = p->f(p->param);

    delete p;
    return ret;
}

Thread::Thread() : thread_(0)
{

}

Thread::~Thread()
{
    this->Detach();
}

bool Thread::Run(ThreadFunc f, void *param)
{
    ThreadParam *p = new ThreadParam();     // 由 DoThreadFunc 释放
    p->f = f;
    p->param = param;

    return pthread_create(&thread_, NULL, DoThreadFunc, p) == 0;
}

bool Thread::Join()
{
    if (thread_ == 0)
        return false;

    int ret = pthread_join(thread_, NULL);
    if (ret == 0) {
        thread_ = 0;
        return true;
    }

    return false;
}

void Thread::Detach()
{
    if (thread_ != 0) {
        pthread_detach(thread_);
        thread_ = 0;
    }
}
