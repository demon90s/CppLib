#pragma once

#include "thread/ThreadQueue.h"
#include "thread/Thread.h"

// 无聊滴输出一些字符
class BoringWriter2
{
public:
    BoringWriter2();
    ~BoringWriter2();

    void Pop(int *v) {
        m_tl.Pop(v);
    }

private:
    static void* WriteThread(void*);
    void DoWrite();
    Thread m_thread[4];
    bool m_is_exit;

    ThreadQueue<int> m_tl;
};

void test_ThreadQueue();