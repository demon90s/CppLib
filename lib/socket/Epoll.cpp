#include "Epoll.h"
#include "Socket.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

Epoll::Epoll(int listen_socketfd, int epoll_size) : listen_socketfd_(listen_socketfd), ep_sz_(epoll_size), callback_(nullptr)
{
    epevt_ = new epoll_event[ep_sz_];
    epfd_ = epoll_create(ep_sz_);

    this->AddEvent(listen_socketfd_, EPOLLIN);
}

Epoll::~Epoll()
{
    for (auto handler : handlers_) {
        Socket::Close(handler->GetSocket());
        delete handler;
    }

    delete[] epevt_;
    Socket::Close(epfd_);

    if (callback_)
        delete callback_;
}

void Epoll::EpollWait(unsigned long timeout_ms)
{
    int evt_num = epoll_wait(epfd_, epevt_, ep_sz_, timeout_ms);
    if (evt_num > 0) {
        this->HandleEvents(evt_num);
    }
}

bool Epoll::Send(NetID netid, const char *data, int len)
{
    if (!handlers_.Exist(netid))
        return false;
    EpollEventHandler *handler = handlers_[netid];

    return handler->OnSend(data, len);
}

NetID Epoll::AddEvent(int socketfd, int evt)
{
    EpollEventHandler *handler = new EpollEventHandler(this, socketfd);

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, socketfd, &ev) != 0) {
        delete handler;
    }

    NetID netid = handlers_.Add(handler);
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
    handlers_.Remove(netid);
}

void Epoll::ModEvent(NetID netid, int evt)
{
    if (!handlers_.Exist(netid))
        return;
    EpollEventHandler *handler = handlers_[netid];

    struct epoll_event ev;
    ev.events = evt;
    ev.data.ptr = handler;
    epoll_ctl(epfd_, EPOLL_CTL_MOD, handler->GetSocket(), &ev);
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
