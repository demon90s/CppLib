#pragma once

#include <unordered_map>
#include <string>

class IModule;

class ModuleManager {
public:
    ModuleManager();
    ~ModuleManager();

    bool RegisterModule(const std::string &name, IModule *module);
    IModule* GetModule(const std::string &name);

    void Run(unsigned long update_ms);

private:
    bool run_;

    typedef std::unordered_map<std::string, IModule*> ModulePool;
    ModulePool module_pool_;
};
