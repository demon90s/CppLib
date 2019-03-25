#include "ModuleManager.h"
#include "IModule.h"
#include "common/clock_functions.h"

#include <csignal>

bool modulemanager_exist = false;
void OnSig(int sig) {
    modulemanager_exist = true;
}

ModuleManager::ModuleManager() : run_(false)
{
    srand(GetTimeMs());
    signal(SIGINT, OnSig);
}

ModuleManager::~ModuleManager()
{
    for (auto &p : module_pool_) {
        delete p.second;
    }
}

bool ModuleManager::RegisterModule(const std::string &name, IModule *module)
{
    if (run_)
        return false;

    if (module_pool_.find(name) != module_pool_.end()) {
        return false;
    }

    module_pool_.emplace(name, module);
    module->SetModuleManager(this);

    module_queue_.push_back(module);

    return true;
}

IModule* ModuleManager::GetModule(const std::string &name)
{
    auto it = module_pool_.find(name);
    if (it == module_pool_.end())
        return nullptr;

    return it->second;
}

void ModuleManager::Run(unsigned long update_ms)
{
    run_ = true;

    // Init
    for (auto it = module_queue_.begin(); it != module_queue_.end();) {
        IModule *module = *it;
        if (!module->Init()) {
            module->Release();
            it = module_queue_.erase(it);
        }
        else {
            ++it;
        }
            
    }

    // Start
    for (auto it = module_queue_.begin(); it != module_queue_.end();) {
        IModule *module = *it;
        if (!module->Start()) {
            module->Release();
            it = module_queue_.erase(it);
        }
        else {
            ++it;
        }
    }

    // Update
    while (!modulemanager_exist) {
        for (auto it = module_queue_.begin(); it != module_queue_.end(); ++it) {
            IModule *module = *it;
            module->Update();
        }

        Sleep(update_ms);
    }
    
    // Release
    for (auto it = module_queue_.rbegin(); it != module_queue_.rend(); ++it) {
        IModule *module = *it;
        module->Release();
    }
}

void ModuleManager::Stop()
{
    modulemanager_exist = true;
}
