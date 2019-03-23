#pragma once

#include "socket/netdef.h"

class NetworkModule;

class INetCallback {
public:
    INetCallback(NetworkModule *network) : network_(network) {}
    virtual ~INetCallback() {}
    virtual void OnAccept(const char *ip, unsigned short port) {}
    virtual void OnConnect(NetID netid, ConnectHandle handle) {}
    virtual void OnRecv(NetID netid, const char *data, int len) {}
    virtual void OnDisconnect(NetID netid) {}

protected:
    NetworkModule *network_;
};
