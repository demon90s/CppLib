#include "MsgQueue.h"
#include "IMsgQCallback.h"
#include <sys/msg.h>
#include <cstring>
#include <cstdio>

#define MAS_Q_MAX_DATA_LEN 1024
struct MsgQStruct {
    long int msg_type = 1;
    char data[MAS_Q_MAX_DATA_LEN];
};

MsgQueue::MsgQueue() : key_(0), msgid_(-1), callback_(nullptr)
{

}

MsgQueue::~MsgQueue()
{
    msgctl(msgid_, IPC_RMID, 0);

    if (callback_)
        delete callback_;
}

bool MsgQueue::Get(int key)
{
    if (msgid_ != -1)
        return false;

    int msgid = msgget(key, 0600 | IPC_CREAT);
    if (msgid == -1) {
        return false;
    }

    msgid_ = msgid;
    key_ = key;

    return true;
}

void MsgQueue::Send(const char* data, int len)
{
    if (len > MAS_Q_MAX_DATA_LEN)
        return;

    MsgQStruct mqs;
    memcpy(mqs.data, data, len);

    if (msgsnd(msgid_, &mqs, len, 0) == -1) {
        perror("msgsend failed");
    }
}

bool MsgQueue::Recv()
{
    MsgQStruct mqs;

    int nrecv = msgrcv(msgid_, &mqs, MAS_Q_MAX_DATA_LEN, mqs.msg_type, 0);
    if (nrecv != -1) {
        if (callback_) {
            callback_->OnRecv(mqs.data, nrecv);
        }

        return true;
    }
    else {
        return false;
    }
}

void MsgQueue::SetCallback(IMsgQCallback *callback)
{
    if (callback_)
        delete callback_;

    callback_ = callback;
}
