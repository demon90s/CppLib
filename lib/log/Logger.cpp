#include "Logger.h"
#include "LogHandler.h"
#include "common/clock_functions.h"
#include <sys/stat.h>
#include <sys/types.h>

Logger::Logger() : is_exist_(true)
{

}

Logger::~Logger()
{
    is_exist_ = true;
    write_thread_.Join();

    for (const auto &p : log_handlers_) {
        delete p.second;
    }
}

bool Logger::Init(const char *path)
{
    if (!is_exist_)
        return false;

    if (mkdir(path, 0700) != 0) {
        struct stat file_stat;
        if (stat(path, &file_stat) != 0)
            return false;

        if (!S_ISDIR(file_stat.st_mode))
            return false;
    }

    path_ = path;
    is_exist_ = false;

    write_thread_.Run(WriteThread, this);

    return true;
}

bool Logger::RegisterLogHandler(const char* catagory, const char* filename, int create_file_interval_s)
{
    if (is_exist_)
        return false;

    if (log_handlers_.count(catagory) > 0)
        return false;

    LogHandler *handler = new LogHandler(catagory);
    if (!handler->Init(path_.c_str(), filename, create_file_interval_s)) {
        delete handler;
        return false;
    }

    log_handlers_.emplace(catagory, handler);
    return true;
}

void Logger::Write(const char *catagory, LogLevel loglevel, const std::string &content)
{
    auto it = log_handlers_.find(catagory);
    if (it == log_handlers_.end())
        return;

    LogHandler *handler = it->second;
    handler->Write(loglevel, content);
}

void* Logger::WriteThread(void *param)
{
    Logger *pthis = (Logger*)param;
    pthis->DoWriteThread();

    return 0;
}

void Logger::DoWriteThread()
{
    while (!is_exist_) {
        for (const auto &p : log_handlers_) {
            LogHandler *handler = p.second;
            handler->TryFlush();
        }

        Sleep(1);
    }
}
