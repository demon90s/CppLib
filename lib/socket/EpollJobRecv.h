#pragma once

#include "IEpollJob.h"

class EpollJobRecv : public IEpollJob {
public:
    EpollJobRecv(NetID netid, const char* data, int len) : IEpollJob(EpollJobType::Recv, netid), data_(data), len_(len) {}
    ~EpollJobRecv() override {
        delete[] data_;
    }

    const char* GetData() const { return data_; }
    int GetLen() const { return len_; }

private:
    const char *data_;
    int len_;
};
