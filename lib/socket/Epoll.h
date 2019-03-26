#pragma once

#include <string>
#include <sys/epoll.h>

#include "common/NoCopy.h"
#include "common/ObjectArray.h"
#include "thread/Thread.h"
#include "thread/ThreadQueue.h"
#include "thread/Mutex.h"
#include "netdef.h"

// 封装 epoll server

#define EPOLL_DEFUALT_SZ 10240

class IEpollJob;
class EpollEventHandler;

class Epoll {
    friend class EpollEventHandler;
public:
    Epoll(int epoll_size = EPOLL_DEFUALT_SZ);
    ~Epoll();

    bool Init(ThreadQueue<IEpollJob*> *job_queue);
    void Release();
    bool StartServer(int listen_socketfd);

    bool Send(NetID netid, const char *data, int len);

    NetID OnConnect(int socketfd);

private:
    NetID AddEvent(int socketfd, int evt);
    void DelEvent(NetID netid, int evt);
    bool ModEvent(NetID netid, int evt);

    static void* EpollWait(void *param);
    void DoEpollWait();
    void HandleEvents(epoll_event *epevt, int evt_num);

    static void* SendThread(void *param);
    void DoSend();

private:
    bool is_exist_;

    Thread epoll_wait_thread_;
    Thread send_thread;

    NoCopy nocpy_;

    int listen_socketfd_;
    int epfd_;
    int ep_sz_;

    ThreadQueue<IEpollJob*> *job_queue_;        // 生产消费队列

    ObjectArray<EpollEventHandler*> handlers_;  // 事件处理对象列表, 索引是 netid TODO 线程不安全
    
    struct DataStruct {
        char *data;
        int len;
        NetID netid;
    };
    ThreadQueue<DataStruct> send_data_queue_;
};
