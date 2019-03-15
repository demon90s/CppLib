#include "test_Thread.h"
#include "common/Sleep.h"
#include <stdio.h>

BoringWriter::BoringWriter()
{
    m_thread.Run(WriteThread, this);
}

void* BoringWriter::WriteThread(void* p)
{
    BoringWriter* p_this = (BoringWriter*)p;
    p_this->DoWrite();

    return NULL;
}

void BoringWriter::DoWrite()
{
    for (int i = 0; i < 5; i++) {
        printf("[%ld] Haha I am so boring\n", time(NULL));
        Sleep(1000);
    }
}

//-----------------------------------------------------------------

void test_Thread()
{
    BoringWriter bw;

    bw.Join();
}