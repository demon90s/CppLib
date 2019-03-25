#pragma once

#include "IEpollJob.h"
#include <string.h>

class EpollJobRecv : public IEpollJob {
public:
    EpollJobRecv(NetID netid, const char* package_data, int package_len) : 
        IEpollJob(EpollJobType::Recv, netid), package_data_(nullptr), package_len_(package_len)
    {
        package_data_ = new char[package_len_];
        memcpy(package_data_, package_data, package_len_);
    }

    ~EpollJobRecv() override {
        delete[] package_data_;
    }

    const char* GetData() const { 
        return package_data_; 
    }
    int GetLen() const { return package_len_; }

private:
    char *package_data_;
    int package_len_;
};
