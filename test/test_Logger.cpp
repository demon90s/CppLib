#include "test_Logger.h"
#include "log/LogAgent.h"
#include "common/clock_functions.h"

void test_Logger()
{
    Logger logger;
    if (!logger.Init("./log")) {
        printf("Logger::Init failed\n");
        return;
    }

    LogAgent stdlog;
    stdlog.Init(&logger, "STDOUT");

    LogAgent boringlog;
    boringlog.Init(&logger, "BoringLog", "Boring");

    for (int i = 0; i < 60 * 5; i ++) {
        stdlog.Printf(LogLevel::Normal, "Hello %d", i);
        boringlog.Printf(LogLevel::Warning, "I am fucking boring %d", i);
        Sleep(1000);
    }
}
