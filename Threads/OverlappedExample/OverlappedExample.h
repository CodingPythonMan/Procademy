#pragma comment(lib, "ws2_32")
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// ���� ���� ������ ���� ����ü�� ����
struct SOCKETINFO {
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

int OverlapMain();

// �񵿱� ����� ó�� �Լ�
unsigned int __stdcall WorkerThread(LPVOID arg);

// ���� ���� �Լ�
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int index);