#pragma once

#include "IEpollJob.h"
#include "netdef.h"

class EpollJobConnect : public IEpollJob {
public:
    EpollJobConnect(NetID netid, ConnectAsynHandle connect_handle) : 
        IEpollJob(EpollJobType::Connect, netid), connect_handle_(connect_handle) {}

    ConnectAsynHandle GetHandle() const { return connect_handle_; }

private:
    ConnectAsynHandle connect_handle_;  // -1 代表失败
};
