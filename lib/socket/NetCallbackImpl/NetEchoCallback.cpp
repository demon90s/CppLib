#include "NetEchoCallback.h"
#include "socket/Epoll.h"

void NetEchoCallback::OnRecv(NetID netid, const char *data, int len)
{
    printf("OnRecv, netid: %d, data: %s, len: %d\n", netid, data, len);
    
    ep_->Send(netid, data, len);    
}
