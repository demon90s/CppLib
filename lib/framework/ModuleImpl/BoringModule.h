#pragma once

#include <iostream>
#include "framework/IModule.h"
#include "common/clock_functions.h"

class BoringModule : public IModule {
public:
    BoringModule() : m_last_update_time(0) {}

    bool Init() override {
        std::cout << "BoringModule::Init" << std::endl;
        return true;
    }

    void Update() override {
        if (GetTimeMs() - m_last_update_time > 1000) {
            std::cout << "BoringModule::Update" << std::endl;
            m_last_update_time = GetTimeMs();
        }
    }

    void Release() override {
        std::cout << "BoringModule::Release" << std::endl;
    }

private:
    unsigned long m_last_update_time;
};
