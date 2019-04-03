#pragma once

#include <set>
#include "log/LogAgent.h"
#include "thread/Thread.h"
#include "thread/Mutex.h"

// 记录使用内存池申请内存的情况
// 采取单例模式

#define MEMORY_LOG_INTERVAL_S /*1*/60          // 硬编码志记间隔

class MemoryPool;

class MemoryLogger {
public:
    MemoryLogger();
    ~MemoryLogger();

    void Register(const MemoryPool *memory_pool);   // 无须反注册了, mempool 都定义成全局的

    bool StartLogger(const char *log_path,
              const char* filename = nullptr, 
              LogAgent::CreateFileInterval create_file_interval_s = LogAgent::CreateFileInterval::Minute);

private:
    static void* LoggerThread(void *arg);
    void DoLoggerThread();

    std::set<const MemoryPool *> memory_pool_list_;

    Logger logger_;
    LogAgent record_log_;

    bool is_exit_;
    Thread logger_thread_;
    Mutex logger_mutex_;
    time_t next_log_time_;
};
