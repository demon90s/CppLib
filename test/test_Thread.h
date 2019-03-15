#pragma once

#include "Thread/Thread.h"

// 无聊滴输出一些字符
class BoringWriter
{
public:
    BoringWriter();

    void Join() { m_thread.Join(); }

private:
    static void* WriteThread(void*);
    void DoWrite();
    Thread m_thread;
};

//------------------------------------------------

void test_Thread();