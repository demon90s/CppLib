#pragma once

#include "framework/IModule.h"
#include "log/Logger.h"
#include "log/LogAgent.h"

class LogModule : public IModule {
public:
    LogModule(const char *path);
    ~LogModule() override;

    bool Init() override;
    bool Start() override;
    void Update() override;
    void Release() override;

    bool RegisterLogAgent(LogAgent& log_agent, 
                          const char* catagory, 
                          const char* filename = nullptr, 
                          LogAgent::CreateFileInterval create_file_interval_s = LogAgent::CreateFileInterval::Minute);

private:
    std::string path_;
    Logger logger_;
};
