#pragma once

#include <string>
#include "thread/ThreadQueue.h"
#include "netdef.h"

class Epoll;

class EpollEventHandler {
public:
    EpollEventHandler(Epoll *ep, int socketfd);
    ~EpollEventHandler();
    void OnAccept(const std::string &ip, unsigned short port);
    void OnCanRead();
    void OnCanWrite();
    bool OnSend(const char *data, int len);

    void SetNetID(NetID netid) { netid_ = netid; }

    Epoll* GetEpoll() const { return ep_; }
    int GetSocket() const { return socketfd_; }
    NetID GetNetID() const { return netid_; }

private:
    Epoll *ep_;
    int socketfd_;
    NetID netid_;

    struct DataStruct {
        char *data;
        int len;
    };
    ThreadQueue<DataStruct> send_data_queue_;
};
