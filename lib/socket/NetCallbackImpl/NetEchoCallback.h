#pragma once

#include "socket/INetCallback.h"
#include <cstdio>

class NetEchoCallback : public INetCallback {
public:
    NetEchoCallback(Epoll *ep) : INetCallback(ep) {}
    ~NetEchoCallback() override {}
    void OnAccept(const char *ip, unsigned short port) override
    {
        printf("OnAccept, ip: %s, port: %d\n", ip, port);
    }

    void OnConnect(NetID netid, ConnectHandle handle) override {}

    void OnRecv(NetID netid, const char *data, int len) override;

    void OnDisconnect(NetID netid) override
    {
        printf("OnDisconnect, netid: %d\n", netid);
    }
};
