#include "EpollEventHandler.h"
#include "Epoll.h"
#include "Socket.h"
#include "EpollJobAccept.h"
#include "EpollJobDisconnect.h"
#include "EpollJobRecv.h"

#include <cstring>

EpollEventHandler::EpollEventHandler(Epoll *ep, int socketfd) : ep_(ep), socketfd_(socketfd), netid_(-1),
    send_data_queue_(256)
{
}

EpollEventHandler::~EpollEventHandler()
{
    DataStruct ds;
    while (send_data_queue_.TryPop(&ds)) {
        delete []ds.data;
    }
}

void EpollEventHandler::OnAccept(int socketfd, const std::string &ip, unsigned short port)
{
    NetID netid = ep_->AddEvent(socketfd, EPOLLIN);

    IEpollJob *job = new EpollJobAccept(ip, port, netid);
    ep_->job_queue_->Push(job);
}

void EpollEventHandler::OnCanRead()
{
    char buffer[1024];
    int len = 0;
    char *data = nullptr;

    while (true) {
        int nread = Socket::Recv(socketfd_, buffer, sizeof(buffer));
        if (nread <= 0) {
            if (len <= 0) {
                IEpollJob *job = new EpollJobDisconnect(netid_);
                ep_->job_queue_->Push(job);

                Socket::Close(socketfd_);
                ep_->DelEvent(netid_, EPOLLIN);
                return;
            }
            break;
        }
        else {
            int tmp_len = len + nread;
            char *tmp_data = new char[tmp_len];
            if (len > 0) {
                memcpy(tmp_data, data, len);
            }
            memcpy(tmp_data + len, buffer, nread);

            if (len > 0) {
                delete []data;
            }

            len = tmp_len;
            data = tmp_data;

            if (nread != static_cast<int>(sizeof(buffer)))
                break;
        }
    }

    bool invalid_package = false;
    char *tmp_data = data;
    if (len > 0 && len <= static_cast<int>(sizeof(int))) {
        invalid_package = true;
    }
    else {
        int package_len = *(int*)(tmp_data);
        while (len >= package_len) {
            IEpollJob *job = new EpollJobRecv(netid_, tmp_data + sizeof(int), package_len);
            ep_->job_queue_->Push(job);

            len = len - sizeof(int) - package_len;
            tmp_data = tmp_data + sizeof(int) + package_len;

            if (len > static_cast<int>(sizeof(int))) {
                package_len = *(int*)(tmp_data);
            }
        }
    }

    if (invalid_package) {
        // 必须头带长度, 处理黏包
        IEpollJob *job = new EpollJobDisconnect(netid_);
        ep_->job_queue_->Push(job);

        Socket::Close(socketfd_);
        ep_->DelEvent(netid_, EPOLLIN);
    }

    if (data)
        delete []data;
}

void EpollEventHandler::OnCanWrite()
{
    DataStruct ds;
    if (!send_data_queue_.TryPop(&ds)) {
        ep_->ModEvent(netid_, EPOLLIN);
        return;
    }

    Socket::Send(socketfd_, ds.data, ds.len);
    delete[] ds.data;
}

void EpollEventHandler::OnSend(const char *data, int len)
{
    // 直接发送, 不修改 epoll 似乎也是可以的
    Socket::Send(socketfd_, data, len);
    delete []data;

    // DataStruct ds;
    // ds.data = data;
    // ds.len = len;

    // if (ep_->ModEvent(netid_, EPOLLIN | EPOLLOUT)) {
    //     send_data_queue_.Push(ds);
    // }
}
