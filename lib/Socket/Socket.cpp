#include "Socket.h"

#include <csignal>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int Socket::CreateSocket()
{
    signal(SIGPIPE, SIG_IGN);
	return socket(AF_INET, SOCK_STREAM, 0);
}

bool Socket::Bind(int socketfd, const char* ip, unsigned short port)
{
	struct sockaddr_in address;

	address.sin_family = AF_INET;

	if (ip == nullptr) {
		address.sin_addr.s_addr = htonl(INADDR_ANY);	
	}
	else {
		address.sin_addr.s_addr = inet_addr(ip);
	}
	address.sin_port = htons(port);

	return bind(socketfd, (struct sockaddr*)&address, sizeof(address)) == 0;
}

bool Socket::Listen(int socketfd, int backlog)
{
    int ret = listen(socketfd, backlog);
    if (ret != 0) return false;

    int enable = 1;
    ret = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (ret != 0) return false;

    return true;
}

int Socket::Accept(int socketfd, char* ip_out, unsigned short *port_out)
{
	struct sockaddr_in address;
	socklen_t address_len;
	int new_socketfd = accept(socketfd, (struct sockaddr*)&address, &address_len);

	if (new_socketfd != -1)
	{
		if (ip_out) 
			strcpy(ip_out, inet_ntoa(address.sin_addr));

		if (port_out)
			*port_out = ntohs(address.sin_port);
	}

	return new_socketfd;
}

bool Socket::Close(int socketfd)
{
	return close(socketfd) == 0;
}

bool Socket::Connect(int socketfd, const char *ip, unsigned short port)
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);

	return connect(socketfd, (struct sockaddr*)&address, sizeof(address)) == 0;
}

int Socket::Send(int socketfd, const void* buf, int size)
{
	return write(socketfd, buf, size);
}

int Socket::Recv(int socketfd, void *buf, int size)
{
	return read(socketfd, buf, size);
}
