#pragma once

#include <string>
#include "thread/ThreadQueue.h"
#include "netdef.h"

class Epoll;

class EpollEventHandler {
public:
    EpollEventHandler(Epoll *ep, int socketfd);
    ~EpollEventHandler();
    void OnAccept(int socketfd, const std::string &ip, unsigned short port);
    void OnCanRead();
    void OnCanWrite();
    void OnSend(const char *data, int len);

    void SetNetID(NetID netid) { netid_ = netid; }

    Epoll* GetEpoll() const { return ep_; }
    int GetSocket() const { return socketfd_; }
    NetID GetNetID() const { return netid_; }

private:
    Epoll *ep_;
    int socketfd_;
    NetID netid_;

    struct DataStruct {
        const char *data;
        int len;
    };
    ThreadQueue<DataStruct> send_data_queue_;
};
