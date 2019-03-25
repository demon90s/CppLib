#pragma once

#include <iostream>
#include "framework/IModule.h"
#include "common/clock_functions.h"
#include "socket/netdef.h"

class NetworkModule;

class EchoClientModule : public IModule {
public:
    EchoClientModule(int send_times = 0) : 
        m_next_echo_time(0), network_(nullptr), server_netid_(-1), connect_server_handle_(-1),
        m_cur_send_times(0), m_send_times(send_times) {}

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

    int m_cur_send_times;
    const int m_send_times;
};
