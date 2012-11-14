
#pragma once

#include <WinSock2.h>
#include <windows.h>

class Server
{
public:
	Server();
	~Server();
	BOOL InitServer();
	BOOL StartServing();
	void EndServing();
	BOOL run();
	static DWORD WINAPI HandleClient(LPVOID lparam);
private:
	USHORT port;
	char ip[32];
	SOCKET m_socket;
};

