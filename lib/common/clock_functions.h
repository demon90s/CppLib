#pragma once

#include <unistd.h>
#include <time.h>

// 挂起执行线程若干毫秒
inline void Sleep(int ms)
{
    usleep(ms * 1000);
}

// 获取当前时间结构 timespec, 并可设置偏移的毫秒
inline struct timespec GetTimeSpec(unsigned long offset_ms = 0)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += offset_ms / 1000;
    ts.tv_nsec += (offset_ms % 1000) * 1000 * 1000; // 1 毫秒 = 1,000,000纳秒

    const long BILLION = 1000000000;
    if (ts.tv_nsec >= BILLION)
    {
        ++ts.tv_sec;
        ts.tv_nsec %= BILLION;
    }
    return ts;
}
