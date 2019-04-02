#include "test_ModuleManager.h"
#include "framework/ModuleManager.h"
#include "framework/ModuleImpl/BoringModule.h"
#include "framework/ModuleImpl/LogModule/LogModule.h"
#include "framework/ModuleImpl/NetwokModule/NetworkModule.h"
#include "framework/ModuleImpl/NetwokModule/NetCallbackImpl/NetEchoCallback.h"
#include "framework/ModuleImpl/EchoClientModule/EchoClientModule.h"
#include "common/string_functions.h"
#include "common/OptHandler.h"

void test_BoringModule()
{
    ModuleManager mm;
    mm.RegisterModule(MakeString(LogModule), new LogModule("./log"));
    mm.RegisterModule("BoringModule", new BoringModule);

    mm.Run(1);
}

void test_NetworkModule()
{
    const char *module_name = MakeString(NetworkModule);

    ModuleManager mm;
    mm.RegisterModule(module_name, new NetworkModule(6789));
    
    NetworkModule* module = dynamic_cast<NetworkModule*>(mm.GetModule(module_name));
    if (module) {
        module->SetCallback(new NetEchoCallback(module));
    }

    mm.Run(1);
}

void test_EchoClientModule()
{
    ModuleManager mm;
    mm.RegisterModule(MakeString(NetworkModule), new NetworkModule());
    mm.RegisterModule(MakeString(EchoClientModule), new EchoClientModule(rand() % 20 + 1));

    mm.Run(1);
}

void test_ModuleManager()
{
    test_BoringModule();
}

void test_Network(int argc, char **argv)
{
    OptHandler oh;
    oh.Parse(argc, argv, {
        {'c', false},
        {'s', false}
    }
    );

    if (oh.HasOpt('s')) {
        test_NetworkModule();
    }
    else if (oh.HasOpt('c')) {
        test_EchoClientModule();
    }
    else {
        printf("usage: %s <-s|-c>\n", argv[0]);
    }

}