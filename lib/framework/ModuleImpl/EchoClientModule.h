#pragma once

#include <iostream>
#include "framework/IModule.h"
#include "common/clock_functions.h"
#include "socket/netdef.h"

class NetworkModule;

class EchoClientModule : public IModule {
public:
    EchoClientModule() : m_next_echo_time(0), network_(nullptr), server_netid_(-1), connect_server_handle_(-1) {}

    bool Init() override;

    bool Start() override;

    void Update() override;

    void Release() override;

    void OnConnect(NetID netid, ConnectHandle handle);

private:
    unsigned long m_next_echo_time;
    NetworkModule *network_;

    NetID server_netid_;
    ConnectHandle connect_server_handle_;
};
