#pragma once

#include "common/NoCopy.h"

// 封装消息队列

class IMsgQCallback;

class MsgQueue {
public:
    MsgQueue();
    ~MsgQueue();
    bool Get(int key);
    void Send(const char* data, int len);
    bool Recv();

    void SetCallback(IMsgQCallback *callback);

private:
    NoCopy nocopy_;
    int key_;
    int msgid_;

    IMsgQCallback *callback_;
};
