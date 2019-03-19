#pragma once

#include "Thread/Thread.h"

// 无聊滴输出一些字符
class BoringWriter
{
public:
    BoringWriter();
    ~BoringWriter();

private:
    static void* WriteThread(void*);
    void DoWrite();
    Thread m_thread;
    bool m_is_exit;
};

//------------------------------------------------

void test_Thread();