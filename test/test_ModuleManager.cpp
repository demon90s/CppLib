#include "test_ModuleManager.h"
#include "framework/ModuleManager.h"
#include "framework/ModuleImpl/BoringModule.h"

void test_ModuleManager()
{
    ModuleManager mm;
    mm.RegisterModule("BoringModule", new BoringModule);

    mm.Run(1);
}