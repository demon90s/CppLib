#pragma once

#include <string>
#include <fstream>
#include <list>
#include "common/NoCopy.h"
#include "thread/Mutex.h"
#include "Logger.h"

// 处理一种日志文件的类型
class LogHandler {
public:
    LogHandler(const char* catagory);
    ~LogHandler();

    bool Init(const char* log_path, const char* filename = nullptr, int create_file_interval_s = 0); // 为空表示标准输出
    void Write(LogLevel loglevel, const std::string &content_);
    void TryFlush();

private:
    static const char* GetLogLevelStr(LogLevel loglevel);
    std::string MakeLogFilePath(bool new_file = false) const;
    void SaveFile();

    NoCopy nocopy_;

    std::ofstream ofs_;
    std::string log_path_;
    std::string filename_;
    std::string catagory_;
    int create_file_interval_s_;
    time_t next_create_file_time_;

    struct LogItem {
        LogItem(LogLevel loglevel, const std::string &content) : loglevel_(loglevel), content_(content) {}
        LogLevel loglevel_;
        std::string content_;
    };
    std::list<LogItem> log_queue_;
    Mutex log_mutex_;
};
