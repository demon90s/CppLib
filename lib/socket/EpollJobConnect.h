#pragma once

#include "IEpollJob.h"
#include "netdef.h"

class EpollJobConnect : public IEpollJob {
public:
    EpollJobConnect(NetID netid, ConnectHandle connect_handle) : 
        IEpollJob(EpollJobType::Connect, netid), connect_handle_(connect_handle) {}

    ConnectHandle GetHandle() const { return connect_handle_; }

private:
    ConnectHandle connect_handle_;  // -1 代表失败
};
