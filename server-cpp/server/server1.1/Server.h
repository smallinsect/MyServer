#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "IServer.h"

#include "Connection.h"

class Server : public IServer {
public:
	int Create(u_short port);
	int Start();
	int Stop();

	static DWORD WINAPI ThreadRun(LPVOID lpParameter);
	static int HandleAPI(SOCKET skt, char* data, int len);
private:
	SOCKET skt;
	sockaddr_in addr;

	u_short port;
	bool bRun;
	HANDLE hThread;
};

