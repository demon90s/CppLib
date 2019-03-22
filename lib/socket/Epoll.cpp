#include "Epoll.h"
#include "Socket.h"

#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

Epoll::Epoll() : listen_socketfd_(-1), epfd_(-1), ep_sz_(0), 
    send_data_queue_(256), connect_queue_(256),
    callback_(nullptr)
{

}

Epoll::~Epoll()
{
    {
        DataStruct ds;
        while (send_data_queue_.TryPop(&ds)) {
            if (handlers_.Exist(ds.netid)) {
                delete []ds.data;
            }
        }
    }

    {
        ConnectStruct cs;
        while (connect_queue_.TryPop(&cs)) {
        }
    }

    for (auto handler : handlers_) {
        Socket::Close(handler->GetSocket());
        delete handler;
    }

    if (epevt_)
        delete[] epevt_;

    if (callback_)
        delete callback_;

    Socket::Close(listen_socketfd_);
    Socket::Close(epfd_);
}

bool Epoll::Init(int listen_socketfd, int epoll_size)
{
    if (epfd_ != -1)
        return false;

    ep_sz_ = epoll_size;
    epevt_ = new epoll_event[ep_sz_];
    epfd_ = epoll_create(ep_sz_);
    listen_socketfd_ = listen_socketfd;

    return this->AddEvent(listen_socketfd_, EPOLLIN) != -1;
}  

void Epoll::EpollWait(unsigned long timeout_ms)
{
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

    // 处理 connection
    {
        ConnectStruct cs;
        while (connect_queue_.TryPop(&cs)) {
            NetID netid = this->AddEvent(cs.socketfd, EPOLLIN);
            if (netid != -1) {
                callback_->OnConnect(netid, cs.handle);
            }
        }
    }

    int evt_num = epoll_wait(epfd_, epevt_, ep_sz_, timeout_ms);

    if (evt_num > 0) {
        this->HandleEvents(evt_num);
    }
}

bool Epoll::Send(NetID netid, const char *data, int len)
{
    DataStruct ds;
    ds.data = new char[len];    // 由 EpollEventHandler 释放
    ds.len = len;
    ds.netid = netid;
    memcpy(ds.data, data, len);

    return send_data_queue_.TryPush(ds);
}

NetID Epoll::OnConnect(int socketfd)
{
    return this->AddEvent(socketfd, EPOLLIN);
}

bool Epoll::ConnectAsny(int socketfd)
{
    ConnectStruct cs;
    cs.socketfd = socketfd;
    cs.handle = connnect_handle_generator.Gen();

    return connect_queue_.TryPush(cs);
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

    handlers_mutex_.Lock();
    NetID netid = handlers_.Add(handler);
    handlers_mutex_.UnLock();

    handler->SetNetID(netid);

    return netid;
}

void Epoll::DelEvent(NetID netid, int evt)
{
    if (!handlers_.Exist(netid))
        return;
    EpollEventHandler *handler = handlers_[netid];

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    epoll_ctl(epfd_, EPOLL_CTL_DEL, handler->GetSocket(), &ev);

    delete handler;

    handlers_mutex_.Lock();
    handlers_.Remove(netid);
    handlers_mutex_.UnLock();
}

bool Epoll::ModEvent(NetID netid, int evt)
{
    if (!handlers_.Exist(netid))
        return false;
    EpollEventHandler *handler = handlers_[netid];

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    return epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->GetSocket(), &ev) == 0;
}

void Epoll::HandleEvents(int evt_num)
{
    for (int i = 0; i < evt_num; i++) {
        EpollEventHandler *handler = (EpollEventHandler*)epevt_[i].data.ptr;
        int fd = handler->GetSocket();
        int ev = epevt_[i].events;

        if ((fd == listen_socketfd_) && (ev & EPOLLIN)) {
            char ip[128];
            unsigned short port;
            int client_socketfd = Socket::Accept(listen_socketfd_, ip, &port);

            if (Socket::SetNonBlock(client_socketfd)) {
                if (client_socketfd != -1) {
                    this->AddEvent(client_socketfd, EPOLLIN);
                    
                    handler->OnAccept(ip, port);

                    callback_->OnAccept(ip, port); 
                }
            }
        }
        else if (ev & EPOLLIN) {
            handler->OnCanRead();
        }
        
        if (ev & EPOLLOUT) {
            handler->OnCanWrite();
        }
    }
}
