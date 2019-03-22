#pragma once

#include <string>
#include <sys/epoll.h>
#include "common/NoCopy.h"
#include "common/ObjectArray.h"
#include "EpollEventHandler.h"
#include "INetCallback.h"

// 封装 epoll server

class Epoll {
    friend class EpollEventHandler;
public:
    Epoll();
    ~Epoll();

    bool Init(int listen_socketfd, int epoll_size = 20480);

    void SetCallback(INetCallback *callback) { callback_ = callback; }
    INetCallback* GetCallback() const { return callback_; }

    void EpollWait(unsigned long timeout_ms);
    bool Send(NetID netid, const char *data, int len);

private:
    bool AddEvent(int socketfd, int evt);
    void DelEvent(NetID netid, int evt);
    void ModEvent(NetID netid, int evt);

    void HandleEvents(int evt_num);

    NoCopy nocpy_;

    int listen_socketfd_;
    int epfd_;
    int ep_sz_;
    epoll_event *epevt_;     // 临时的事件列表

    ObjectArray<EpollEventHandler*> handlers_;       // 事件处理对象列表, 索引是 netid

    INetCallback *callback_;
};
