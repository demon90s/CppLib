#include "test_Epoll.h"
#include "socket/Epoll.h"
#include "socket/Socket.h"
#include "common/other_macros.h"
#include "common/OptHandler.h"
#include "common/clock_functions.h"
#include "common/string_functions.h"
#include "thread/ThreadQueue.h"
#include "socket/EpollJobAccept.h"
#include "socket/EpollJobDisconnect.h"
#include "socket/EpollJobRecv.h"

#include <cstring>
#include <csignal>

#define SERVER_PORT 6789

static bool exist = false;
void onsig(int sig) {
    exist = true;
}

static void server()
{
    printf("server\n");
    signal(SIGINT, onsig);

    int listen_socketfd = Socket::CreateSocket();
    
    if (!Socket::Bind(listen_socketfd, nullptr, SERVER_PORT)) {
        perror("Socket::Bind failed");
        return;
    }

    if (!Socket::Listen(listen_socketfd)) {
        perror("Socket::Listen failed");
        return;
    }

    printf("[DEBUG] listen succ\n");

    Epoll ep;
    ThreadQueue<IEpollJob*> job_queue(1);

    if (!ep.Init(listen_socketfd, &job_queue)) {
        perror("Epoll Init failed");
        Socket::Close(listen_socketfd);
        return;
    }
    printf("[DEBUG] server start\n");

    IEpollJob *job;
    while (!exist) {
        if (job_queue.TryPop(&job)) {
            if (job->GetType() == EpollJobType::Accept) {
                EpollJobAccept *accept_job = (EpollJobAccept*)job;
                printf("a client accept, netid: %d, ip: %s, port: %u\n", accept_job->GetNetID(), accept_job->GetIp().c_str(), accept_job->GetPort());
            }
            else if (job->GetType() == EpollJobType::Disconnect) {
                printf("a client disconnect, netid: %d\n", job->GetNetID());
            }
            else if (job->GetType() == EpollJobType::Recv) {
                EpollJobRecv *recv_job = (EpollJobRecv*)job;
                printf("Recv msg from netid[%d]: %s\n", recv_job->GetNetID(), recv_job->GetData());
                ep.Send(recv_job->GetNetID(), recv_job->GetData(), recv_job->GetLen());
            }

            delete job;
        }
        Sleep(1);
    }

    printf("[DEBUG] server stop\n");
}

static void client()
{
    printf("client\n");

    int socketfd = Socket::CreateSocket();

    if (!Socket::Connect(socketfd, "127.0.0.1", SERVER_PORT)) {
        perror("Socket::Connnect failed");
        return;
    }

    for (int i = 0; i < 100; i++) {
        char buffer[1024];
        StringFormat2(buffer, sizeof(buffer), "hello[%d]", i + 1);
        int sendlen = Socket::Send(socketfd, buffer, strlen(buffer) + 1);
        if (sendlen < 0) {
            perror("Socket::Send");
            break;
        }

        printf("Send: %s\n", buffer);

        int nread = Socket::Recv(socketfd, buffer, sizeof(buffer));
        if (nread != static_cast<int>(strlen(buffer) + 1)) {
            if (nread == 0) {
                printf("server disconnect\n");
            }
            else
                perror("Socket::Recv");
                
            break;
        }

        printf("Recv: %s\n", buffer);

        Sleep(1000);
    }

    Socket::Close(socketfd);
}

void test_Epoll(int argc, char **argv)
{
    IGNORE_NOUSED_WARNING(server);
    IGNORE_NOUSED_WARNING(client);

    OptHandler oh;
    const std::vector<Opt> opts = {
        {'s', false},
        {'c', false}
    }; 
    if (!oh.Parse(argc, argv, opts)) {
        printf("%s\n", oh.ErrorDesc().c_str());
        return;
    }

    if (oh.HasOpt('s')) {
        server();
    }
    else if (oh.HasOpt('c')) {
        client();
    }
    else {
        printf("usage: %s -s|-c\n", argv[0]);
    }
}
