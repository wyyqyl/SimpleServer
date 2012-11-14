// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "server.h"

#pragma comment(lib, "Ws2_32.lib")

Server::Server()
{
	port = 27015;
	memcpy(ip, "127.0.0.1", sizeof(ip));
}

Server::~Server()
{
	EndServing();
}

BOOL Server::InitServer()
{
	//initialize Winsock
	WSADATA wsaData;

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iRet != NO_ERROR)
	{
		printf("Error at WSAStartup()\n");
		return FALSE;
	}

	//create a socket
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		printf("Error at socket():%ld\n",WSAGetLastError());
		WSACleanup();
		return FALSE;
	}

	//bind a socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip);
	service.sin_port = htons(port);

	if(bind(m_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(m_socket);
		WSACleanup();
		return FALSE;
	}
	else
		printf("bind OK.\n");

	//listen on a socket
	if(listen(m_socket, 20) == SOCKET_ERROR)
	{
		printf("Error listening on socket.\n");
		return FALSE;
	}
	else
		printf("listening ok.\n");

	return TRUE;
}

BOOL Server::StartServing()
{
	//accept a connection
	SOCKET AcceptSocket;

	printf("Waiting for a client to connect...\n");

	while(1)
	{
		AcceptSocket = SOCKET_ERROR;
		while(AcceptSocket == SOCKET_ERROR)
			AcceptSocket = accept(m_socket, NULL, NULL);

		printf("Client Connected.\n");

		DWORD dwThreadId;
		HANDLE hThread;

		hThread = CreateThread(NULL, NULL, HandleClient, (LPVOID)AcceptSocket, 0, &dwThreadId);
		if(hThread == NULL)
			printf("CreatThread AnswerThread() failed.\n");
		else
			printf("CreateThread OK.\n");

	}
}

void Server::EndServing()
{
	printf("Server shutting down...\n");
	closesocket(m_socket);
	WSACleanup();
}

DWORD WINAPI Server::HandleClient(LPVOID lparam)
{
	SOCKET ClientSocket = (SOCKET)lparam;
	int tid = GetCurrentThreadId();

	while (true)
	{
		char buffer[1024] = { 0 };
		char time[64] = { 0 };
		int bytesReceived = SOCKET_ERROR;

		bytesReceived = recv(ClientSocket, buffer, sizeof(buffer), 0);

		if (bytesReceived == 0 || bytesReceived == SOCKET_ERROR)
			break;

		SYSTEMTIME lt;
		GetLocalTime(&lt);
		sprintf_s(time, " %4d/%02d/%02d %02d:%02d:%02d.%03d", lt.wYear, lt.wMonth, lt.wDay,
			lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
		strcat_s(buffer, time);

		send(ClientSocket, buffer, strlen(buffer), 0);
		printf("Thread[%d] %s\n", tid, buffer);
	}
	printf("Thread[%d] terminated\n", tid);

	return 0;
}

BOOL Server::run()
{
	if (!InitServer())
	{
		printf("Failed to init server.\n");
		return FALSE;
	}
	StartServing();
	return TRUE;
}