#pragma once

#include "framework/IModule.h"
#include "socket/Epoll.h"
#include "thread/Thread.h"

class NetworkModule : public IModule {
public:
    NetworkModule();
    ~NetworkModule() override;

    bool Init() override;
    bool Update() override;
    void Release() override;

    Epoll *GetEpoll() { return &ep_; }

    bool StartServer(unsigned short listen_port, std::string &error_msg);

    void SetCallback(INetCallback *callback);

    bool Send(NetID netid, const char *data, int len);

    bool Connect(const char *ip, unsigned short port, unsigned long timeout_ms, NetID *netid_out);
    bool ConnectAsyn(const char *ip, unsigned short port);

private:
    static void *EpWork(void*);
    void DoEpWork();

    bool is_exist_;
    bool server_start_;
    Epoll ep_;
    Thread ep_work_thread_;
};
