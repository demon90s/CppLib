#include "NetworkModule.h"
#include "socket/Socket.h"
#include "common/string_functions.h"
#include "common/clock_functions.h"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>

NetworkModule::NetworkModule() : is_exist_(false), server_start_(false)
{

}

NetworkModule::~NetworkModule()
{
    if (server_start_)
        ep_work_thread_.Join();
}

bool NetworkModule::Init()
{
    std::cout << "NetworkModule::Init" << std::endl;
    
    return true;
}

bool NetworkModule::Update()
{
    return true;
}

void NetworkModule::Release()
{
    std::cout << "NetworkModule::Release" << std::endl;

    is_exist_ = true;
    return;
}

bool NetworkModule::StartServer(unsigned short listen_port, std::string &error_msg)
{
    if (server_start_) {
        error_msg = "Server already started";
        return false;
    }

    int listen_socketfd = Socket::CreateSocket();
    
    if (!Socket::Bind(listen_socketfd, nullptr, listen_port)) {
        error_msg = StringFormat("Socket::Bind failed: %s", strerror(errno));
        return false;
    }

    if (!Socket::Listen(listen_socketfd)) {
        error_msg = StringFormat("Socket::Listen failed: %s", strerror(errno));
        return false;
    }

    if (!ep_.Init(listen_socketfd)) {
        error_msg = StringFormat("Epoll::Init failed: %s", strerror(errno));
        Socket::Close(listen_socketfd);
        return false;
    }

    ep_work_thread_.Run(EpWork, this);
    server_start_ = true;

    return true;
}

void NetworkModule::SetCallback(INetCallback *callback)
{
    ep_.SetCallback(callback);
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

    if (!Socket::Connect(socketfd, ip, port)) {
        return false;
    }

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

bool NetworkModule::ConnectAsyn(const char *ip, unsigned short port)
{
    return false; // TODO
}

void *NetworkModule::EpWork(void* param)
{
    NetworkModule *pthis = (NetworkModule*)param;

    pthis->DoEpWork();

    return 0;
}

void NetworkModule::DoEpWork()
{
    while (!is_exist_) {
        ep_.EpollWait(1);
    }
}
