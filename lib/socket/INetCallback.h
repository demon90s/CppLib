#pragma once

#include "netdef.h"

class Epoll;

class INetCallback {
public:
    INetCallback(Epoll *ep) : ep_(ep) {}
    virtual ~INetCallback() {}
    virtual void OnAccept(const char *ip, unsigned short port) {}
    virtual void OnRecv(NetID netid, const char *data, int len) {}
    virtual void OnDisconnect(NetID netid) {}

protected:
    Epoll *ep_;
};
