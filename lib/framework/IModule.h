#pragma once

class ModuleManager;

class IModule {
public:
    virtual ~IModule() {}

    void SetModuleManager(ModuleManager *module_mgr) { module_mgr_ = module_mgr; }
    ModuleManager* GetModuleManager() const { return module_mgr_; }

    virtual bool Init() = 0;
    virtual void Update() = 0;
    virtual void Release() = 0;

protected:
    ModuleManager *module_mgr_;
};
