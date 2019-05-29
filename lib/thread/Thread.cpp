#include "Thread.h"
#include "common/other_templates.h"

struct ThreadParam {
    Thread::ThreadFunc f;
    void *param;
};

static void* DoThreadFunc(void *param)
{
    ThreadParam *p = (ThreadParam*)param;
    AutoDeletor<ThreadParam> ad(p);

    void *ret = p->f(p->param);

    pthread_exit(ret);
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
    if (thread_ != 0)
        return false;

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

bool Thread::Detach()
{
    if (thread_ != 0) {
        if (pthread_detach(thread_) == 0) {
            thread_ = 0;
            return true;
        }
    }

    return false;
}

bool Thread::Cancel()
{
    int res = pthread_cancel(thread_);
    if (res != 0) {
        return false;
    }

    return this->Join();
}
