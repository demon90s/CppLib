#pragma once

#include <unistd.h>

// 挂起执行线程若干毫秒
inline void Sleep(int ms)
{
    usleep(ms * 1000);
}