#pragma once

#include "IEpollJob.h"
#include "netdef.h"
#include <string>

class EpollJobAccept : public IEpollJob {
public:
    EpollJobAccept(const std::string &ip, unsigned short port, NetID netid) : IEpollJob(EpollJobType::Accept, netid),
        ip_(ip), port_(port)
    {

    }

    const std::string& GetIp() const { return ip_; }
    unsigned short GetPort() const { return port_; }

private:
    std::string ip_;
    unsigned short port_;
};
