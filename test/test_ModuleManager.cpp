#include "test_ModuleManager.h"
#include "framework/ModuleManager.h"
#include "framework/ModuleImpl/BoringModule.h"
#include "framework/ModuleImpl/NetwokModule/NetworkModule.h"
#include "common/string_functions.h"
#include "socket/NetCallbackImpl/NetEchoCallback.h"

void test_BoringModule()
{
    ModuleManager mm;
    mm.RegisterModule("BoringModule", new BoringModule);

    mm.Run(1);
}

void test_NetworkModule()
{
    const char *module_name = MakeString(NetworkModule);

    ModuleManager mm;
    mm.RegisterModule(module_name, new NetworkModule());

    NetworkModule* module = dynamic_cast<NetworkModule*>(mm.GetModule(module_name));
    if (module) {
        module->SetCallback(new NetEchoCallback(module->GetEpoll()));

        std::string error_msg;
        if (!module->StartServer(6789, error_msg)) {
            std::cout << "StartServer failed: " << error_msg << std::endl;
        }
        else {
            std::cout << "StartServer succ.\n";
        }
    }

    mm.Run(1);
}

void test_ModuleManager()
{
    //test_BoringModule();
    test_NetworkModule();
}