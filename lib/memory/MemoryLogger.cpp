#include "MemoryLogger.h"
#include "MemoryPool.h"
#include "common/string_functions.h"

MemoryLogger::MemoryLogger() : is_exit_(true), next_log_time_(0)
{
    next_log_time_ = time(NULL) + MEMORY_LOG_INTERVAL_S;
}

MemoryLogger::~MemoryLogger()
{
    is_exit_ = true;
    logger_thread_.Join();
}

void* MemoryLogger::LoggerThread(void *arg)
{
    MemoryLogger* pthis = (MemoryLogger*)arg;

    pthis->DoLoggerThread();

    return 0;
}

void MemoryLogger::DoLoggerThread()
{
    while (!is_exit_) {
        if (time(NULL) < next_log_time_)
            continue;        

        // [block_name:used_block_count/total_block_count], ...
        std::string log_str;

        logger_mutex_.Lock();
        for (const MemoryPool * memory_pool : memory_pool_list_) {
            log_str += StringFormat("[%s: %d/%d], ", memory_pool->GetName(), memory_pool->GetUsedBlockCount(), memory_pool->GetTotalBlockCount());
        }
        logger_mutex_.UnLock();

        if (log_str.length() > 2)
            log_str.resize(log_str.length() - 2);   // erase last ', '

        if (log_str.length() > 0)
            record_log_.Printf(LogLevel::Normal, "%s", log_str.c_str());

        next_log_time_ = time(NULL) + MEMORY_LOG_INTERVAL_S;
    }
}

bool MemoryLogger::StartLogger(const char *log_path, 
                        const char* filename, 
                        LogAgent::CreateFileInterval create_file_interval_s)
{
    bool ret = logger_.Init(log_path);
    if (!ret)
        return false;

    ret = record_log_.Init(&logger_, "MemoryLog", filename, create_file_interval_s);
    if (!ret)
        return false;

    is_exit_ = false;

    logger_thread_.Run(LoggerThread, this);

    return ret;
}

void MemoryLogger::Register(const MemoryPool *memory_pool)
{
    logger_mutex_.Lock();
    memory_pool_list_.insert(memory_pool);
    logger_mutex_.UnLock();
}
