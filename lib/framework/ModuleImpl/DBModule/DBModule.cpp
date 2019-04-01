#include "DBModule.h"
#include "framework/ModuleManager.h"
#include "framework/ModuleImpl/LogModule/LogModule.h"
#include "common/clock_functions.h"
#include <iostream>

DBModule::DBModule(const char* user_name, const char* passwd, const char* db_name) :
    user_name_(user_name), passwd_(passwd), db_name_(db_name), is_exist(true)
{

}

DBModule::~DBModule()
{
    is_exist = true;
    workthread.Join();
}

bool DBModule::Init()
{
    LogModule *log_module = dynamic_cast<LogModule*>(this->GetModuleManager()->GetModule(MakeString(LogModule)));
    if (!log_module) {
        std::cerr << "DBModule::Init failed, Cant not get LogModule\n";
        return false;;
    }

    if (!log_module->RegisterLogAgent(stdlog_, "DBLog")) {
        std::cerr << "DBModule::Init failed, Register Log failed\n";
        return false;
    }

    bool ret = mysql_handler_.Connect("127.0.0.1", user_name_.c_str(), passwd_.c_str(), db_name_.c_str());
    if (!ret) {
        stdlog_.Printf(LogLevel::Error, "DBModule::Init failed, Connect to db failed, %s", mysql_handler_.ErrorDesc().c_str());
        return false;
    }

    is_exist = false;
    workthread.Run(WorkThread, this);

    stdlog_.Printf(LogLevel::Normal, "DBModule::Init succ");

    return ret;
}

bool DBModule::Start()
{
    return true;
}

void DBModule::Update()
{

}

void DBModule::Release()
{
    stdlog_.Printf(LogLevel::Normal, "DBModule::Realease");
}

void* DBModule::WorkThread(void *arg)
{
    DBModule *pthis = (DBModule*)arg;
    pthis->DoWorkThread();

    return 0;
}

void DBModule::DoWorkThread()
{
    while (!is_exist) {
        Sleep(1);
    }
}
