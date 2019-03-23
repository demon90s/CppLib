#pragma once

#include "framework/IModule.h"
#include "socket/Epoll.h"
#include "thread/Thread.h"
#include "thread/ThreadQueue.h"
#include "socket/IEpollJob.h"
#include "INetCallback.h"

// 网络模块
// 如果给予一个监听端口, 那么自动成为一个服务端, 否则后续可以主动调用 StartServer 成为服务端

class NetworkModule : public IModule {
public:
    NetworkModule(unsigned short listen_port = 0);
    ~NetworkModule() override;

    bool Init() override;
    bool Start() override { return true; }
    void Update() override;
    void Release() override;

    bool StartServer(unsigned short listen_port, std::string &error_msg);

    void SetCallback(INetCallback *callback);

    bool Send(NetID netid, const char *data, int len);

    bool Connect(const char *ip, unsigned short port, unsigned long timeout_ms, NetID *netid_out);
    ConnectHandle ConnectAsyn(const char *ip, unsigned short port, unsigned long timeout_ms);

private:
    static void* ConnectAsynWork(void *param);
    void DoConnectAsynWork();

    unsigned short listen_port_;
    bool is_exist_;
    Epoll ep_;
    INetCallback *callback_;

    ThreadQueue<IEpollJob*> job_queue_;

    Thread connect_asyn_thread[4];

    struct ConnectStruct {
        std::string ip;
        unsigned short port;
        ConnectHandle handle;
        unsigned long timeout_ms; 
    };
    ThreadQueue<ConnectStruct> connect_queue_;
    Mutex connect_mutex_;
};
