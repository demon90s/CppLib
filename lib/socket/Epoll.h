#pragma once

#include <string>
#include <sys/epoll.h>
#include "common/NoCopy.h"
#include "common/ObjectArray.h"
#include "thread/Thread.h"
#include "EpollEventHandler.h"
#include "netdef.h"

// 封装 epoll server

class IEpollJob;

class Epoll {
    friend class EpollEventHandler;
public:
    Epoll(int epoll_size = 20480);
    ~Epoll();

    // 初始化并启动服务线程
    bool Init(int listen_socketfd, ThreadQueue<IEpollJob*> *job_queue);

    bool Send(NetID netid, const char *data, int len);

    NetID OnConnect(int socketfd);
    bool ConnectAsny(int socketfd);

private:
    NetID AddEvent(int socketfd, int evt);
    void DelEvent(NetID netid, int evt);
    bool ModEvent(NetID netid, int evt);

    static void* EpollWait(void *param);
    void DoEpollWait();
    void HandleEvents(epoll_event *epevt, int evt_num);

    bool is_exist_;

    Thread epoll_wait_thread_;

    NoCopy nocpy_;

    int listen_socketfd_;
    int epfd_;
    int ep_sz_;

    ThreadQueue<IEpollJob*> *job_queue_;        // 生产消费队列

    ObjectArray<EpollEventHandler*> handlers_;  // 事件处理对象列表, 索引是 netid
    
    struct DataStruct {
        char *data;
        int len;
        NetID netid;
    };
    ThreadQueue<DataStruct> send_data_queue_;
};
