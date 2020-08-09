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

	//�����׽���
	skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		printf("[server] socket error ...\n");
		return -1;
	}
	printf("[server] socket success ...\n");
	//��������Ϣ
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	//�׽��ְ󶨶˿ں�ip
	if (bind(skt, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[server] bind error ...\n");
		return -1;
	}
	printf("[server] bind success ...\n");

	//��ʼ�����׽���
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
		//�ͻ�����Ϣ
		sockaddr_in caddr = { 0 };
		int caddrlen = sizeof(sockaddr_in);
		SOCKET cskt = accept(s->skt, (sockaddr*)&caddr, &caddrlen);//���ܿͻ�������
		if (cskt == SOCKET_ERROR) {
			printf("[server] accept error ...\n");
			return -1;
		}
		printf("[server] accept success ...\n");
		//��ӡ���ӵĿͻ�����Ϣ
		printf("[client] ip:%s port:%d connect ...\n", inet_ntoa(caddr.sin_addr), ntohs(caddr.sin_port));

		Connection *c = new Connection();
		c->Create(cskt, caddr, HandleAPI, cid);
		c->Start();
		cid++;
	}

	return 0;
}

int Server::HandleAPI(SOCKET skt, char* data, int len) {
	printf("�����ͻ��˻�дҵ��...");
	if (send(skt, data, len, 0) == SOCKET_ERROR) {
		printf("��������ʧ��\n");
		return -1;
	}
	return 0;
}