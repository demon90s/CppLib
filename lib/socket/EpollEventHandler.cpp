#include "EpollEventHandler.h"
#include "Epoll.h"
#include "Socket.h"
#include "EpollJobAccept.h"
#include "EpollJobDisconnect.h"
#include "EpollJobRecv.h"

#include <cstring>

EpollEventHandler::EpollEventHandler(Epoll *ep, int socketfd) : ep_(ep), socketfd_(socketfd), netid_(-1),
    send_data_queue_(256)
{
}

EpollEventHandler::~EpollEventHandler()
{
    DataStruct ds;
    while (send_data_queue_.TryPop(&ds)) {
        delete []ds.data;
    }
}

void EpollEventHandler::OnAccept(int socketfd, const std::string &ip, unsigned short port)
{
    NetID netid = ep_->AddEvent(socketfd, EPOLLIN);

    IEpollJob *accept_job = new EpollJobAccept(ip, port, netid);
    ep_->job_queue_->Push(accept_job);
}

void EpollEventHandler::OnCanRead()
{
    char buffer[1024];
    int len = 0;
    char *data;

    while (true) {
        int nread = Socket::Recv(socketfd_, buffer, sizeof(buffer));
        if (nread <= 0) {
            if (len <= 0) {
                IEpollJob *job = new EpollJobDisconnect(netid_);
                ep_->job_queue_->Push(job);

                Socket::Close(socketfd_);
                ep_->DelEvent(netid_, EPOLLIN);
            }
            break;
        }
        else {
            int tmp_len = len + nread;
            char *tmp_data = new char[tmp_len];
            if (len > 0) {
                memcpy(tmp_data, data, len);
            }
            memcpy(tmp_data + len, buffer, nread);

            if (len > 0) {
                delete []data;
            }

            len = tmp_len;
            data = tmp_data;

            if (nread != static_cast<int>(sizeof(buffer)))
                break;
        }
    }

    if (len > 0) {
        IEpollJob *job = new EpollJobRecv(netid_, data, len);
        ep_->job_queue_->Push(job);
    }
}

void EpollEventHandler::OnCanWrite()
{
    DataStruct ds;
    if (!send_data_queue_.TryPop(&ds)) {
        ep_->ModEvent(netid_, EPOLLIN);
        return;
    }

    Socket::Send(socketfd_, ds.data, ds.len);
    delete[] ds.data;
}

void EpollEventHandler::OnSend(const char *data, int len)
{
    DataStruct ds;
    ds.data = data;
    ds.len = len;

    if (ep_->ModEvent(netid_, EPOLLIN | EPOLLOUT)) {
        send_data_queue_.TryPush(ds);
    }
}
