#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// ���� ���� ������ ���� ����ü
struct SOCKETINFO
{
	OVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

int IocpMain();

// �۾��� ������ �Լ�
unsigned int WINAPI WorkerThread003(LPVOID arg);