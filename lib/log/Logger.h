#pragma once

#include "common/NoCopy.h"
#include "thread/Thread.h"
#include <string>
#include <unordered_map>

enum class LogLevel {
    Normal, Warning, Error
};

class LogHandler;

class Logger {
public:
    Logger();
    ~Logger();

    bool Init(const char *path);

    bool RegisterLogHandler(const char* catagory, const char* filename = nullptr, int create_file_interval_s = 0);

    void Write(const char *catagory, LogLevel loglevel,  const std::string &content);

private:
    static void* WriteThread(void *param);
    void DoWriteThread();

    NoCopy nocopy_;

    std::string path_;      // log 的输出目录
    std::unordered_map<std::string, LogHandler*> log_handlers_;

    bool is_exist_;
    Thread write_thread_;
};
