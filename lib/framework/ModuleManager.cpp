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
    for (auto it = module_pool_.begin(); it != module_pool_.end(); ) {
        IModule *module = it->second;

        if (!module->Init()) {
            module->Release();
            delete module;
            it = module_pool_.erase(it);
        }
        else {
            ++it;
        }
    }

    // Update
    while (!modulemanager_exist) {
        for (auto it = module_pool_.begin(); it != module_pool_.end(); ) {
            IModule *module = it->second;

            if (!module->Update()) {
                module->Release();
                delete module;
                it = module_pool_.erase(it);
            }
            else {
                ++it;
            }
        }

        if (module_pool_.empty())
            break;

        Sleep(update_ms);
    }
    
    // Release
    for (auto it = module_pool_.begin(); it != module_pool_.end(); ++it) {
        IModule *module = it->second;
        module->Release();
    }
}
