#pragma once

#include <unistd.h>
#include <sys/time.h>
#include <ctime>
#include <string>

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

// 获取毫秒级别时间
inline unsigned long GetTimeMs()
{
    struct timezone tz = { 0, 0 };
	timeval time;
	gettimeofday(&time, &tz);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// 获取时间描述 格式固定是 YYYY-MM-DD HH:MM:SS
inline void GetAscTime(char buffer[], size_t len)
{
    time_t t;
    time(&t);

	struct tm *p_tm = localtime(&t);

	strftime(buffer, len, "%Y-%m-%d %H:%M:%S", p_tm);
}

inline std::string GetAscTime()
{
    char asc_time[128];
    GetAscTime(asc_time, sizeof(asc_time));
    return asc_time;
}
