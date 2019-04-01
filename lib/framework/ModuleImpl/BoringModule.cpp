#include "BoringModule.h"
#include "framework/ModuleImpl/LogModule/LogModule.h"
#include "framework/ModuleManager.h"
#include "common/string_functions.h"

bool BoringModule::Init() 
{
    LogModule *log_module = dynamic_cast<LogModule*>(this->GetModuleManager()->GetModule(MakeString(LogModule)));
    if (!log_module) {
        std::cerr << "BoringModule::Init failed, Cant not get LogModule\n";
        return false;;
    }

    if (!log_module->RegisterLogAgent(stdlog_, "BoringLog")) {
        std::cerr << "BoringModule::Init failed, Register Log failed\n";
        return false;
    }

    stdlog_.Printf(LogLevel::Normal, "BoringModule::Init succ");

    return true;
}

bool BoringModule::Start() 
{
    return true;
}

void BoringModule::Update() 
{
    if (GetTimeMs() - m_last_update_time > 1000) {
        //std::cout << "BoringModule::Update" << std::endl;

        //stdlog_.Printf(LogLevel::Normal, "BoringModule::Update");

        m_last_update_time = GetTimeMs();
    }
}

void BoringModule::Release() 
{
    //std::cout << "BoringModule::Release" << std::endl;

    stdlog_.Printf(LogLevel::Normal, "BoringModule::Release");
}
