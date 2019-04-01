#pragma once

#include <iostream>
#include "framework/IModule.h"
#include "common/clock_functions.h"
#include "log/LogAgent.h"

class BoringModule : public IModule {
public:
    BoringModule() : m_last_update_time(0) {}

    bool Init() override;

    bool Start() override;

    void Update() override;

    void Release() override;

private:
    unsigned long m_last_update_time;

    LogAgent stdlog_;
};
