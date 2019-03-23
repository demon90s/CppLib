#pragma once

class ModuleManager;

class IModule {
public:
    virtual ~IModule() {}

    void SetModuleManager(ModuleManager *module_mgr) { module_mgr_ = module_mgr; }
    ModuleManager* GetModuleManager() const { return module_mgr_; }

    virtual bool Init() = 0;                // Run后调用一次
    virtual bool Start() = 0;               // Init成功则调用一次
    virtual void Update() = 0;              // Start成功则每帧调用一次
    virtual void Release() = 0;             // 结束前调用, Init 或 Start 失败提前调用

protected:
    ModuleManager *module_mgr_;
};
