#include "framework/IModule.h"
#include "db/MySqlHandler.h"
#include "log/LogAgent.h"
#include "thread/Thread.h"

class DBModule : public IModule {
public:
    DBModule(const char* user_name, const char* passwd, const char* db_name);
    ~DBModule() override;

    bool Init() override;
    bool Start() override;
    void Update() override;
    void Release() override;

private:
    static void* WorkThread(void *arg);
    void DoWorkThread();

    MySqlHandler mysql_handler_;

    std::string user_name_;
    std::string passwd_;
    std::string db_name_;

    LogAgent stdlog_;
    bool is_exist;
    Thread workthread;
};

