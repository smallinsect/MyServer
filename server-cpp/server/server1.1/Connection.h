#pragma once


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>

#include "IConnection.h"

typedef int (*HandleFunc)(SOCKET skt, char* data, int len);

class Connection : public IConnection {
public:
	int Create(SOCKET skt, sockaddr_in addr, HandleFunc handle_api, int connID);
	int Start();
	int Stop();

	static DWORD WINAPI StartReader(LPVOID lpParameter);

private:
	SOCKET skt;
	sockaddr_in addr;
	HandleFunc handle_api;
	bool isClose;
	int connID;
};

