#include "NetworkModule.h"
#include "socket/Socket.h"
#include "common/string_functions.h"
#include "common/array_functions.h"
#include "common/clock_functions.h"
#include "socket/EpollJobAccept.h"
#include "socket/EpollJobDisconnect.h"
#include "socket/EpollJobRecv.h"
#include "socket/EpollJobConnect.h"

#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>

NetworkModule::NetworkModule(unsigned short listen_port) : listen_port_(listen_port), is_exist_(false), callback_(nullptr), 
    job_queue_(256), connect_queue_(4)
{
}

NetworkModule::~NetworkModule()
{
    if (!is_exist_)
        this->Release();
}

bool NetworkModule::Init()
{
    std::cout << "NetworkModule::Init" << std::endl;

    for (int i = 0; i < ELEM_NUM(connect_asyn_thread); i++)
        connect_asyn_thread[i].Run(ConnectAsynWork, this);

    std::string error_msg;
    if (!ep_.Init(&job_queue_)) {
        error_msg = StringFormat("Epoll::Init failed: %s", strerror(errno));
        std::cerr << error_msg << std::endl;
        return false;
    }

    if (listen_port_ > 0) {
        if (!this->StartServer(listen_port_, error_msg)) {
            std::cerr << error_msg << std::endl;
            return false;
        }
    }
    
    return true;
}

void NetworkModule::Update()
{
    IEpollJob *job;
    while (job_queue_.TryPop(&job)) {
        switch (job->GetType())
        {
            case EpollJobType::Accept: {
                EpollJobAccept *accept_job = (EpollJobAccept*)job;

                if (callback_)
                    callback_->OnAccept(accept_job->GetIp().c_str(), accept_job->GetPort());
            }
            break;

            case EpollJobType::Recv: {
                EpollJobRecv *recv_job = (EpollJobRecv*)job;

                if (callback_)
                    callback_->OnRecv(recv_job->GetNetID(), recv_job->GetData(), recv_job->GetLen());
            }
            break;

            case EpollJobType::Connect: {
                EpollJobConnect *connect_job = (EpollJobConnect*)job;

                if (callback_)
                    callback_->OnConnect(connect_job->GetNetID(), connect_job->GetHandle());
            }
            break;

            case EpollJobType::Disconnect: {
                EpollJobDisconnect *disconnect_job = (EpollJobDisconnect*)job;
                
                if (callback_)
                    callback_->OnDisconnect(disconnect_job->GetNetID());
            }
            break;
        }

        delete job;
    }
}

void NetworkModule::Release()
{
    std::cout << "NetworkModule::Release" << std::endl;

    is_exist_ = true;

    ep_.StopServer();

    for (int i = 0; i < ELEM_NUM(connect_asyn_thread); i++)
        connect_asyn_thread[i].Join();

    ConnectStruct cs;
    while (connect_queue_.TryPop(&cs)) {
    }

    // ? Epoll ???, ???? job ? Push ?
    IEpollJob *job;
    while (job_queue_.TryPop(&job)) {
        delete job;
    }

    if (callback_)
        delete callback_;

    return;
}

bool NetworkModule::StartServer(unsigned short listen_port, std::string &error_msg)
{
    int listen_socketfd = Socket::CreateSocket();
    
    if (!Socket::Bind(listen_socketfd, nullptr, listen_port)) {
        error_msg = StringFormat("Socket::Bind failed: %s", strerror(errno));
        return false;
    }

    if (!Socket::Listen(listen_socketfd)) {
        error_msg = StringFormat("Socket::Listen failed: %s", strerror(errno));
        return false;
    }

    if (!ep_.StartServer(listen_socketfd)) {
        error_msg = StringFormat("Epoll::StartServer failed: %s", strerror(errno));
        Socket::Close(listen_socketfd);
        return false;
    }

    return true;
}

void NetworkModule::SetCallback(INetCallback *callback)
{
    callback_ = callback;
}

bool NetworkModule::Send(NetID netid, const char *data, int len)
{
    return ep_.Send(netid, data, len);
}

bool NetworkModule::Connect(const char *ip, unsigned short port, unsigned long timeout_ms, NetID *netid_out)
{
    int socketfd = Socket::CreateSocket();

    if (!Socket::SetNonBlock(socketfd)) {
        return false;
    }

    Socket::Connect(socketfd, ip, port);

    // 检测超时
    {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        fd_set fdwrite;
        FD_ZERO(&fdwrite);
        FD_SET(socketfd, &fdwrite);

        int ret = select(socketfd + 1, nullptr, &fdwrite, nullptr, &tv);
        if (ret > 0 && FD_ISSET(socketfd, &fdwrite))
        {
            int error = 0;
            int size = sizeof(int);

            if (0 == Socket::GetSockOpt(socketfd, SOL_SOCKET, SO_ERROR, (char*)&error, &size) && error == 0) {
                int netid = ep_.OnConnect(socketfd);
                if (netid_out && netid != -1) {
                    *netid_out = netid;
                    return true;
                }
                else {
                    Socket::Close(socketfd);
                    return false;
                }
            }
        }
    }

    Socket::Close(socketfd);
    return false;
}

ConnectHandle NetworkModule::ConnectAsyn(const char *ip, unsigned short port, unsigned long timeout_ms)
{
    static ConnectHandle handle_gen = 0;

    connect_mutex_.Lock();
    ++handle_gen;

    ConnectStruct cs;
    cs.handle = handle_gen;
    cs.ip = ip;
    cs.port = port;
    cs.timeout_ms = timeout_ms;
    connect_queue_.Push(cs);

    connect_mutex_.UnLock();

    return cs.handle;
}

void* NetworkModule::ConnectAsynWork(void *param)
{
    NetworkModule *pthis = (NetworkModule*)param;
    pthis->DoConnectAsynWork();

    return 0;
}

void NetworkModule::DoConnectAsynWork()
{
    ConnectStruct cs;
    NetID netid;

    while (!is_exist_) {
        while (connect_queue_.TryPop(&cs)) {
            bool ret = this->Connect(cs.ip.c_str(), cs.port, cs.timeout_ms, &netid);

            if (!ret)
                netid = -1;

            EpollJobConnect *job = new EpollJobConnect(netid, cs.handle);
            job_queue_.Push(job);
        }

        Sleep(1);
    }
}
