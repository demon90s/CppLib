#include "test_ThreadQueue.h"
#include "common/clock_functions.h"
#include "common/array_functions.h"
#include <iostream>

BoringWriter2::BoringWriter2() : m_is_exit(false), m_tl(2)
{
    m_is_exit = false;

    //m_thread[0].Run(WriteThread, this);

    // 启动4个线程测试, 生产速度提升4倍
    for (int i = 0; i < ELEM_NUM(m_thread); i++)
        m_thread[i].Run(WriteThread, this);
}

BoringWriter2::~BoringWriter2()
{
    m_is_exit = true;

    //m_thread[0].Join();

    for (int i = 0; i < ELEM_NUM(m_thread); i++)
        m_thread[i].Join();
}

void* BoringWriter2::WriteThread(void* p)
{
    BoringWriter2* p_this = (BoringWriter2*)p;
    p_this->DoWrite();

    return NULL;
}

void BoringWriter2::DoWrite()
{
    while (!m_is_exit)
    {
        m_tl.Push(666);
        Sleep(1000);
    }
}

//-----------------------------------------------------------------

void test_ThreadQueue()
{
    BoringWriter2 bw2;

    int v;
    for (int i = 0; i < 10; ++i) {
        bw2.Pop(&v);
        std::cout << i << " " << v << std::endl;
    }
}
