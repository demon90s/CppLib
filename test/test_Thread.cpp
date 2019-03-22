#include "test_Thread.h"
#include "common/clock_functions.h"
#include <stdio.h>

BoringWriter::BoringWriter() : m_is_exit(false)
{
    m_is_exit = false;
    m_thread.Run(WriteThread, this);
}

BoringWriter::~BoringWriter()
{
    m_is_exit = true;
    m_thread.Join();
}

void* BoringWriter::WriteThread(void* p)
{
    BoringWriter* p_this = (BoringWriter*)p;
    p_this->DoWrite();

    return NULL;
}

void BoringWriter::DoWrite()
{
    while (!m_is_exit)
    {
        printf("[%ld] Haha I am so boring\n", time(NULL));
        Sleep(1000);
    }
}

//-----------------------------------------------------------------

void test_Thread()
{
    BoringWriter bw;

    Sleep(5000);
}