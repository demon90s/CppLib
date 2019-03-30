#pragma once

#include "Logger.h"
#include <stdarg.h>

class LogAgent {
public:
    LogAgent() {}
    
    enum class CreateFileInterval {
        Minute = 60,
        TenMinute = 60 * 10,
        Hour = 60 * 60,
        Day = 60 * 60 * 24
    };
    bool Init(Logger *logger, const char* catagory, const char* filename = nullptr, CreateFileInterval create_file_interval_s = CreateFileInterval::Minute) {
        logger_ = logger;
        catagory_ = catagory;
        return logger_->RegisterLogHandler(catagory_.c_str(), filename, static_cast<int>(create_file_interval_s));
    }

    void Printf(LogLevel loglevel, const char *fmt, ...);

private:
    Logger *logger_;
    std::string catagory_;
};
