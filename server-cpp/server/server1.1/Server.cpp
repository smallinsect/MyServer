#include "Server.h"

int Server::Create(u_short port) {
	this->port = port;
	return 0;
}

int Server::Start() {
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0) {
		printf("[server] WSAStartup error ...\n");
		return -1;
	}
	printf("[server] WSAStartup success ...\n");

	//创建套接字
	skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");
	//服务器信息
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	//套接字绑定端口和ip
	if (bind(skt, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//开始监听套接字
	if (listen(skt, SOMAXCONN) == SOCKET_ERROR) {
		printf("[server] listen error ...\n");
		return -1;
	}
	printf("[server] listen success ...\n");

	bRun = true;
	hThread = CreateThread(NULL, 0, ThreadRun, this, 0, NULL);
}

int Server::Stop() {
	closesocket(skt);
	WSACleanup();
	return 0;
}

DWORD Server::ThreadRun(LPVOID lpParameter) {
	Server* s = (Server*)lpParameter;
	int cid = 0;
	while (s->bRun) {
		//客户端信息
		sockaddr_in caddr = { 0 };
		int caddrlen = sizeof(sockaddr_in);
		SOCKET cskt = accept(s->skt, (sockaddr*)&caddr, &caddrlen);//接受客户端连接
		if (cskt == SOCKET_ERROR) {
			printf("[server] accept error ...\n");
			return -1;
		}
		printf("[server] accept success ...\n");
		//打印连接的客户端信息
		printf("[client] ip:%s port:%d connect ...\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

		Connection *c = new Connection();
		c->Create(cskt, caddr, HandleAPI, cid);
		c->Start();
		cid++;
	}

	return 0;
}

int Server::HandleAPI(SOCKET skt, char* data, int len) {
	printf("开启客户端回写业务...");
	if (send(skt, data, len, 0) == SOCKET_ERROR) {
		printf("发送数据失败\n");
		return -1;
	}
	return 0;
}