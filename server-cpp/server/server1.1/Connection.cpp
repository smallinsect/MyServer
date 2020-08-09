#include "Connection.h"


int Connection::Create(SOCKET skt, sockaddr_in addr, HandleFunc handle_api, int connID) {
	this->skt = skt;
	this->addr = addr;
	this->handle_api = handle_api;
	this->isClose = false;
	this->connID = connID;
	return 0;
}

int Connection::Start() {

	HANDLE hThread = CreateThread(NULL, 0, StartReader, this, 0, NULL);

	return 0;
}

int Connection::Stop() {
	closesocket(skt);
	return 0;
}

DWORD Connection::StartReader(LPVOID lpParameter) {
	Connection* c = (Connection*)lpParameter;
	while (true) {
		char buf[512] = "";
		if (recv(c->skt, buf, sizeof(buf), 0) == SOCKET_ERROR) {
			printf("client exit ...");
			return -1;
		}
		printf("%s ... %d\n", buf, strlen(buf));

		c->handle_api(c->skt, buf, strlen(buf));
	}

	return 0;
}