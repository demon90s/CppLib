#include "NetEchoCallback.h"
#include "framework/ModuleImpl/NetwokModule/NetworkModule.h"
#include "socket/Epoll.h"

void NetEchoCallback::OnRecv(NetID netid, const char *data, int len)
{
    printf("OnRecv, netid: %d, data: %s, len: %d\n", netid, data, len);
    
    network_->Send(netid, data, len);    
}
