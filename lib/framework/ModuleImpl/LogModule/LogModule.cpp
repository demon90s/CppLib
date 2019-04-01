#include "LogModule.h"

LogModule::LogModule(const char *path) : path_(path)
{

}

LogModule::~LogModule()
{

}

bool LogModule::Init()
{
    bool ret = logger_.Init(path_.c_str());
    if (!ret) {
        return false;
    }

    return true;
}

bool LogModule::Start()
{
    return true;
}

void LogModule::Update()
{

}

void LogModule::Release()
{

}

bool LogModule::RegisterLogAgent(LogAgent& log_agent, 
                          const char* catagory, 
                          const char* filename, 
                          LogAgent::CreateFileInterval create_file_interval_s)
{
    return log_agent.Init(&logger_, catagory, filename, create_file_interval_s);
}
