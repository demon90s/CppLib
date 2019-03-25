#include <iostream>
#include "test_Thread.h"
#include "test_ObjectArray.h"
#include "test_OptHandler.h"
#include "test_XmlHandler.h"
#include "test_string_functions.h"
#include "test_ThreadQueue.h"
#include "test_Epoll.h"
#include "test_ModuleManager.h"

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

    return 0;
}
