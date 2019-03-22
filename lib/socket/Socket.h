#pragma once

// 封装 socket api ，使用的 domain 是 AF_INET ， type 是 tcp

class Socket
{
public:
	// 获取 socket fd
	static int CreateSocket();

	// 命名 socket
	static bool Bind(int socketfd, const char *ip, unsigned short port);

	// 监听 socket
    static bool Listen(int socketfd, int backlog = 5);

	// 接收连接，返回新的 socket fd
	static int Accept(int socketfd, char* ip_out = 0, unsigned short *port_out = 0);

	// 关闭 socket fd
	static bool Close(int socketfd);

	// 请求连接目标地址 ip:port
	static bool Connect(int socketfd, const char *ip, unsigned short port);

	// 发送消息，返回发送的字节数
	static int Send(int socketfd, const void* buf, int size);

	// 接收消息，返回接收的字节数，如果是0，代表断开连接，返回-1代表出错
	static int Recv(int socketfd, void *buf, int size);

	// 设置非阻塞IO
	static bool SetNonBlock(int socketfd);
};
