#pragma once

#include "IEpollJob.h"
#include "netdef.h"

class EpollJobDisconnect : public IEpollJob {
public:
    EpollJobDisconnect(NetID netid) : IEpollJob(EpollJobType::Disconnect, netid) {}
};