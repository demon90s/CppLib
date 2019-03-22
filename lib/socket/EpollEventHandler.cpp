#include "EpollEventHandler.h"
#include "Epoll.h"
#include "Socket.h"
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

void EpollEventHandler::OnAccept(const std::string &ip, unsigned short port)
{
    
}

void EpollEventHandler::OnCanRead()
{
    char buffer[1];
    int len = 0;
    char *data;

    while (true) {
        int nread = Socket::Recv(socketfd_, buffer, sizeof(buffer));
        if (nread <= 0) {
            if (len > 0)
                break;
            
            ep_->GetCallback()->OnDisconnect(netid_);

            Socket::Close(socketfd_);
            ep_->DelEvent(netid_, EPOLLIN);

            if (len > 0) {
                delete []data;
                len = 0;
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
        ep_->GetCallback()->OnRecv(netid_, data, len);
        delete []data;
    }
}

void EpollEventHandler::OnCanWrite()
{
    DataStruct ds;
    if (!send_data_queue_.TryPop(&ds)) {
        return;
    }

    Socket::Send(socketfd_, ds.data, ds.len);
    delete[] ds.data;

    ep_->ModEvent(netid_, EPOLLIN);
}

bool EpollEventHandler::OnSend(const char *data, int len)
{
    DataStruct ds;
    ds.data = new char[len];
    ds.len = len;
    memcpy(ds.data, data, len);

    ep_->ModEvent(netid_, EPOLLIN | EPOLLOUT);
    
    return send_data_queue_.TryPush(ds);
}
