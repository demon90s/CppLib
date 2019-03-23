#pragma once

enum class EpollJobType {
    Accept,
    Recv,
    Connect,
    Disconnect,
};

class IEpollJob {
public:
    IEpollJob(EpollJobType type, NetID netid) : type_(type), netid_(netid) {}
    virtual ~IEpollJob() {}

    EpollJobType GetType() const { return type_; }
    NetID GetNetID() const { return netid_; }

protected:
    EpollJobType type_;
    NetID netid_;
};
