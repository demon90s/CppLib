#pragma once

#include "framework/IModule.h"
#include "socket/Epoll.h"
#include "thread/Thread.h"
#include "thread/ThreadQueue.h"
#include "socket/IEpollJob.h"
#include "INetCallback.h"

class NetworkModule : public IModule {
public:
    NetworkModule();
    ~NetworkModule() override;

    bool Init() override;
    void Update() override;
    void Release() override;

    Epoll *GetEpoll() { return &ep_; }

    bool StartServer(unsigned short listen_port, std::string &error_msg);

    void SetCallback(INetCallback *callback);

    bool Send(NetID netid, const char *data, int len);

    // TODO
    bool Connect(const char *ip, unsigned short port, unsigned long timeout_ms, NetID *netid_out);
    bool ConnectAsyn(const char *ip, unsigned short port);

private:
    bool is_exist_;
    Epoll ep_;
    INetCallback *callback_;

    ThreadQueue<IEpollJob*> job_queue_;
};
