#include "EchoClientModule.h"
#include "framework/ModuleImpl/NetwokModule/NetworkModule.h"
#include "framework/ModuleManager.h"
#include "framework/ModuleImpl/NetwokModule/INetCallback.h"
#include "common/string_functions.h"

//------------------------------------------

class EchoClientNetCallback : public INetCallback {
public:
    EchoClientNetCallback(NetworkModule *network, EchoClientModule *echo_client) : INetCallback(network), echo_client_(echo_client) {}
    void OnDisconnect(NetID netid) override {
        std::cerr << "Server Stop, stop\n";
        echo_client_->GetModuleManager()->Stop();
    }

private:
    EchoClientModule *echo_client_;
};

//------------------------------------------

bool EchoClientModule::Init()
{
    std::cout << "EchoClientModule::Init" << std::endl;

    return true;
}

bool EchoClientModule::Start()
{
    NetworkModule* network = dynamic_cast<NetworkModule*>(this->GetModuleManager()->GetModule(MakeString(NetworkModule)));
    if (!network) {
        std::cerr << "EchoClientModule::Start failed, NetworkModule not registered\n";
        return false;
    }
    network_ = network;

    network_->SetCallback(new EchoClientNetCallback(network_, this));

    if (!network_->Connect("127.0.0.1", 6789, 1000, &server_netid_)) {
        std::cerr << "EchoClientModule::Start failed, cannot connect server\n";
        return false;
    }

    std::cout << "EchoClientModule::Start, Connect server succ, netid: " << server_netid_ << std::endl;

    return true;
}

void EchoClientModule::Update()
{
    if (GetTimeMs() - m_next_echo_time < 1000) {
        return;
    }

    char buffer[] = "hello";
    network_->Send(server_netid_, buffer, sizeof(buffer));

    m_next_echo_time = GetTimeMs();
}

void EchoClientModule::Release()
{
    std::cout << "EchoClientModule::Release" << std::endl;
}