#include "test_MsgQueue.h"
#include "ipc/MsgQueue.h"
#include "ipc/IMsgQCallback.h"
#include "common/OptHandler.h"
#include "common/clock_functions.h"
#include <stdio.h>
#include <string.h>

class MsgQEchoCallback : public IMsgQCallback {
public:
    ~MsgQEchoCallback() override {}

    void OnRecv(const char* data, int len) override {
        printf("Recv: data: %s, len: %d\n", data, len);
        if (strcmp(data, "end") == 0)
            exit(0);
    }
};

void mq_recver()
{
    MsgQueue mq;
    if (!mq.Get(888)) {
        perror("MsgQueue::Get");
        return;
    }

    mq.SetCallback(new MsgQEchoCallback());
    while (mq.Recv()) {
    }
}

void mq_sender()
{
    MsgQueue mq;
    if (!mq.Get(888)) {
        perror("MsgQueue::Get");
        return;
    }

    char data[] = "hello";
    for (int i = 0; i < 10; i++) {
        mq.Send(data, sizeof(data));
        Sleep(1000);
    }
}

void test_MsgQueue(int argc, char** argv)
{
    OptHandler oh;
    if (!oh.Parse(argc, argv, {{'s', false}, {'r', false}})) {
        printf("Opt Parse failed: %s\n", oh.ErrorDesc().c_str());
        return;
    }

    if (oh.HasOpt('s')) {
        mq_sender();
    }
    else if (oh.HasOpt('r')) {
        mq_recver();
    }
    else {
        printf("usage: %s <-s|-r>\n", argv[0]);
    }
}
