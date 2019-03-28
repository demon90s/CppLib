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

    void Write(LogLevel loglevel, const char *fmt, ...) {
        int buffer_len = 1024;
        char *buffer = new char[buffer_len]{};
        int str_len = -1;

        do
        {
            va_list ap;
            va_start(ap, fmt);
            str_len = vsnprintf(buffer, buffer_len, fmt, ap);

            if (str_len >= buffer_len) {
                str_len = -1;

                char *tmp_buffer = new char[buffer_len * 2]{};
                delete[] buffer;
                buffer_len *= 2;
                buffer = tmp_buffer;
            }
            va_end(ap);
        } while (str_len == -1);

        std::string str = buffer;
        delete[] buffer;

        logger_->Write(catagory_.c_str(), loglevel, str);
    }

private:
    Logger *logger_;
    std::string catagory_;
};
