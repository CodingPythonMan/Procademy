#pragma comment(lib, "ws2_32")
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// ���� ���� ������ ���� ����ü�� ����
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};


// �񵿱� ����� ���۰� ó�� �Լ�
unsigned int WINAPI WorkerThread002(LPVOID arg);
void CALLBACK CompletionRoutine(
	DWORD dwError, DWORD Transferred,
	LPWSAOVERLAPPED lpOverlapped, DWORD Flags
);

int OverlapMain002();