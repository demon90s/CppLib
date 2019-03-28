#include "LogHandler.h"
#include "common/clock_functions.h"
#include "common/string_functions.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

LogHandler::LogHandler(const char* catagory) : 
    catagory_(catagory), create_file_interval_s_(0), next_create_file_time_(0)
{

}

LogHandler::~LogHandler()
{
    next_create_file_time_ = time(NULL);
    this->TryFlush();

    if (ofs_.is_open())
        ofs_.close();
}

bool LogHandler::Init(const char* log_path, const char *filename, int create_file_interval_s)
{
    log_path_ = log_path;

    if (filename) {
        if (ofs_.is_open())
            return false;

        filename_ = filename;
        create_file_interval_s_ = create_file_interval_s;

        std::string log_dir = log_path;
        log_dir += '/';
        log_dir += catagory_;
        if (mkdir(log_dir.c_str(), 0700) != 0) {
            struct stat file_stat;
            if (stat(log_dir.c_str(), &file_stat) != 0)
                return false;

            if (!S_ISDIR(file_stat.st_mode))
                return false;
        }

        std::string log_file = this->MakeLogFilePath();
        ofs_.open(log_file, std::fstream::app);
        if (!ofs_) {
            ofs_.clear();
            return false;
        }

        this->SaveFile();
        next_create_file_time_ = time(NULL) + create_file_interval_s_;
    }

    return true;
}

void LogHandler::Write(LogLevel loglevel, const std::string &content_)
{
    log_mutex_.Lock();
    log_queue_.emplace_back(loglevel, content_);
    log_mutex_.UnLock();
}

void LogHandler::TryFlush()
{
    time_t now_time = time(NULL);

    if (log_queue_.size() <= 0)
        return;

    if (now_time >= next_create_file_time_) {
        this->SaveFile();
        next_create_file_time_ = time(NULL) + create_file_interval_s_;
    }

    std::list<LogItem> log_queue_tmp;
    log_mutex_.Lock();
    log_queue_.swap(log_queue_tmp);
    log_mutex_.UnLock();

    for (const LogItem &log_item : log_queue_tmp) {
        // [time] catagory (loglevel): content\n
        std::string log_str = StringFormat("[%s] %s (%s): %s\n", 
            GetAscTime().c_str(), catagory_.c_str(), GetLogLevelStr(log_item.loglevel_), log_item.content_.c_str());

        if (ofs_.is_open())
            ofs_ << log_str << std::flush;
        else {
            std::cout << log_str << std::flush;
        }
    }
}

const char* LogHandler::GetLogLevelStr(LogLevel loglevel)
{
    switch (loglevel)
    {
        case LogLevel::Normal:
            return "Normal";
    
        case LogLevel::Warning:
            return "Warning";

        case LogLevel::Error:
            return "Error";
    }
    return "";
}

std::string LogHandler::MakeLogFilePath(bool new_file) const
{
    if (new_file) {
        char suffix[128];
        {
            time_t t;
            time(&t);
            struct tm *p_tm = localtime(&t);
            strftime(suffix, sizeof(suffix), "%Y_%m_%d_%H_%M_%S", p_tm);
        }

        return StringFormat("%s/%s/%s_%s.txt", log_path_.c_str(), catagory_.c_str(), filename_.c_str(), suffix);
    }
    else {
        return StringFormat("%s/%s/%s.txt", log_path_.c_str(), catagory_.c_str(), filename_.c_str());
    }
}

void LogHandler::SaveFile()
{
    if (!ofs_.is_open())
        return;

    std::string old_log_file = this->MakeLogFilePath();
    std::string log_file = this->MakeLogFilePath(true);
    
    log_mutex_.Lock();
    ofs_.close();
    rename(old_log_file.c_str(), log_file.c_str());
    ofs_.open(old_log_file, std::fstream::trunc);
    log_mutex_.UnLock();
}
