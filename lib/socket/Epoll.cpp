#include "Epoll.h"
#include "Socket.h"
#include "EpollEventHandler.h"
#include "common/clock_functions.h"

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

Epoll::Epoll(int epoll_size) : is_exist_(true), listen_socketfd_(-1), epfd_(-1), ep_sz_(epoll_size), 
    send_data_queue_(epoll_size)
{

}

Epoll::~Epoll()
{
    if (!is_exist_)
        this->Release();
}

bool Epoll::Init(ThreadQueue<IEpollJob*> *job_queue)
{
    if (epfd_ != -1)
        return false;

    epfd_ = epoll_create(ep_sz_);
    if (epfd_ == -1)
        return false;

    job_queue_ = job_queue;

    is_exist_ = false;
    epoll_wait_thread_.Run(EpollWait, this);

    return true;
}

void Epoll::Release()
{
    listen_socketfd_ = -1;

    is_exist_ = true;
    epoll_wait_thread_.Join();

    {
        DataStruct ds;
        while (send_data_queue_.TryPop(&ds)) {
            if (handlers_.Exist(ds.netid)) {
                delete []ds.data;
            }
        }
    }

    for (auto handler : handlers_) {
        Socket::Close(handler->GetSocket());
        delete handler;
    }

    Socket::Close(epfd_);
}

bool Epoll::StartServer(int listen_socketfd)
{
    listen_socketfd_ = listen_socketfd;

    if (this->AddEvent(listen_socketfd_, EPOLLIN) == -1) {
        return false;
    }

    return true;
}

bool Epoll::Send(NetID netid, const char *data, int len)
{
    DataStruct ds;
    ds.data = new char[sizeof(int) + len];    // 由 EpollEventHandler 释放, 头带长度, 处理黏包
    ds.len = sizeof(int) + len;
    ds.netid = netid;
    *(int*)ds.data = len;
    memcpy(ds.data + sizeof(int), data, len);

    send_data_queue_.Push(ds);
    return true;
}

NetID Epoll::OnConnect(int socketfd)
{
    NetID netid;
    netid = this->AddEvent(socketfd, EPOLLIN);

    return netid;
}

NetID Epoll::AddEvent(int socketfd, int evt)
{
    EpollEventHandler *handler = new EpollEventHandler(this, socketfd);

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, socketfd, &ev) != 0) {
        delete handler;
        return -1;
    }

    NetID netid = handlers_.Add(handler);
    handler->SetNetID(netid);

    return netid;
}

void Epoll::DelEvent(NetID netid, int evt)
{
    if (!handlers_.Exist(netid)) {
        return;
    }
    EpollEventHandler *handler = handlers_[netid];

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    epoll_ctl(epfd_, EPOLL_CTL_DEL, handler->GetSocket(), &ev);

    delete handler;

    handlers_.Remove(netid);
}

bool Epoll::ModEvent(NetID netid, int evt)
{
    if (!handlers_.Exist(netid)) {
        return false;
    }
    EpollEventHandler *handler = handlers_[netid];

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    bool ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->GetSocket(), &ev) == 0;

    return ret;
}

void* Epoll::EpollWait(void *param)
{
    Epoll *pthis = (Epoll*)param;

    pthis->DoEpollWait();

    return 0;
}

void Epoll::DoEpollWait()
{
    epoll_event *epevt = new epoll_event[ep_sz_];

    while (!is_exist_) {
        // 处理 send queue
        {
            DataStruct ds;
            while (send_data_queue_.TryPop(&ds)) {
                if (handlers_.Exist(ds.netid)) {
                    EpollEventHandler *handler = handlers_[ds.netid];
                    handler->OnSend(ds.data, ds.len);
                }
            }
        }

        int evt_num = epoll_wait(epfd_, epevt, ep_sz_, 0);
        if (evt_num > 0) {
            this->HandleEvents(epevt, evt_num);
        }

        Sleep(1);
    }

    delete[] epevt;
}

void Epoll::HandleEvents(epoll_event *epevt, int evt_num)
{
    for (int i = 0; i < evt_num; i++) {
        EpollEventHandler *handler = (EpollEventHandler*)epevt[i].data.ptr;
        int fd = handler->GetSocket();
        int ev = epevt[i].events;

        if (ev & EPOLLOUT) {
            handler->OnCanWrite();
        }

        if ((fd == listen_socketfd_) && (ev & EPOLLIN)) {
            char ip[128];
            unsigned short port;
            int client_socketfd = Socket::Accept(listen_socketfd_, ip, &port);
            if (client_socketfd != -1) {
                if (Socket::SetNonBlock(client_socketfd)) {
                    handler->OnAccept(client_socketfd, ip, port);
                }
            }
        }
        else if (ev & EPOLLIN) {
            handler->OnCanRead();
        }
    }
}
