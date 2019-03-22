#pragma once

#include <string>
#include <sys/epoll.h>
#include "common/NoCopy.h"
#include "common/ObjectArray.h"
#include "EpollEventHandler.h"
#include "INetCallback.h"
#include "netdef.h"

// 封装 epoll server

class EpollConnectHandleGenerater {
public:
    ConnectHandle Gen() {
        ConnectHandle handle_ret;
        mutex_.Lock();
        handle_ret = ++handle_;
        mutex_.UnLock();
        return handle_ret;
    }

private:
    ConnectHandle handle_ = 0;
    Mutex mutex_;
};

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

    NetID OnConnect(int socketfd);
    bool ConnectAsny(int socketfd);

private:
    NetID AddEvent(int socketfd, int evt);
    void DelEvent(NetID netid, int evt);
    bool ModEvent(NetID netid, int evt);

    void HandleEvents(int evt_num);

    NoCopy nocpy_;

    int listen_socketfd_;
    int epfd_;
    int ep_sz_;
    epoll_event *epevt_;     // 临时的事件列表

    Mutex handlers_mutex_;
    ObjectArray<EpollEventHandler*> handlers_;       // 事件处理对象列表, 索引是 netid
    
    struct DataStruct {
        char *data;
        int len;
        NetID netid;
    };
    ThreadQueue<DataStruct> send_data_queue_;

    struct ConnectStruct {
        int socketfd;
        ConnectHandle handle;
    };
    ThreadQueue<ConnectStruct> connect_queue_;
    EpollConnectHandleGenerater connnect_handle_generator;

    INetCallback *callback_;
};
