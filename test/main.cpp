#include <iostream>
#include "test_Thread.h"
#include "test_ObjectArray.h"
#include "test_OptHandler.h"
#include "test_XmlHandler.h"
#include "test_string_functions.h"
#include "test_ThreadQueue.h"
#include "test_Epoll.h"
#include "test_ModuleManager.h"
#include "test_Logger.h"
#include "test_MsgQueue.h"
#include "test_MySqlHandler.h"
#include "test_Memory.h"
#include "test_Heap.h"

int main(int argc, char *argv[])
{
    //test_Thread();
    //test_ObjectArray();
    //test_OptHandler(argc, argv);
    //test_XmlHandler();
    //test_string_functions();
    //test_ThreadQueue();
    //test_Epoll(argc, argv);
    //test_ModuleManager();
    test_Network(argc, argv);
    //test_Logger();
    //test_MsgQueue(argc, argv);
    //test_MySqlHandler();
    //test_Memory();
    //test_Heap();

    return 0;
}
