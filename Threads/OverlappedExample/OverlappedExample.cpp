#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
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

int TotalSockets = 0;
SOCKETINFO* SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION cs;

// �񵿱� ����� ó�� �Լ�
unsigned int WINAPI WorkerThread(LPVOID arg);

// ���� ���� �Լ�
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int index);

int main()
{
	int retval;
	InitializeCriticalSection(&cs);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return 1;

	// bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 1;

	// listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return 1;

	// ���� �̺�Ʈ ��ü ����
	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT)
		return 1;

	EventArray[TotalSockets++] = hEvent;

	// ������ ����
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	if (hThread == NULL)
		return 1;
	CloseHandle(hThread);

	// ������ ��ſ� ����� ����
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	DWORD recvbytes, flags;

	while (1)
	{
		// accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
			break;

		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// ���� ���� �߰�
		if (AddSocketInfo(clientSock) == FALSE)
		{
			closesocket(clientSock);
			printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			continue;
		}

		// �񵿱� ����� ����
		SOCKETINFO* ptr = SocketInfoArray[TotalSockets - 1];
		flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				RemoveSocketInfo(TotalSockets - 1);
				continue;
			}
		}

		// ������ ���� ��ȭ�� �˸�
		WSASetEvent(EventArray[0]);
	}

	// ���� ����
	WSACleanup();
	DeleteCriticalSection(&cs);

	return 0;
}

// �񵿱� ����� ó�� �Լ�
unsigned int __stdcall WorkerThread(LPVOID arg)
{
	int retval;

	while (1)
	{
		// �̺�Ʈ ��ü ����
		int index = WSAWaitForMultipleEvents(TotalSockets,
			EventArray, FALSE, WSA_INFINITE, FALSE);

		if (index == WSA_WAIT_FAILED)
			continue;

		index -= WSA_WAIT_EVENT_0;
		WSAResetEvent(EventArray[index]);
		if (index == 0)
			continue;

		// Ŭ���̾�Ʈ ���� ���
		SOCKETINFO* ptr = SocketInfoArray[index];
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

		// �񵿱� ����� ��� Ȯ��
		DWORD Transferred, flags;
		retval = WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
			&Transferred, FALSE, &flags);

		if (retval == FALSE || Transferred == 0)
		{
			RemoveSocketInfo(index);
			printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
			
			continue;
		}

		// ������ ���۷� ����
		if (ptr->recvbytes == 0)
		{
			ptr->recvbytes = Transferred;
			ptr->sendbytes = 0;
			// ���� ������ ���
			ptr->buf[ptr->recvbytes] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientAddr.sin_addr),
				ntohs(clientAddr.sin_port), ptr->buf);
		}
		else
		{
			ptr->sendbytes += Transferred;
		}

		if (ptr->recvbytes > ptr->sendbytes)
		{
			// ������ ������
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					return 1;

				continue;
			}
		}
		else
		{
			ptr->recvbytes = 0;

			// ������ �ޱ�
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			flags = 0;
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
					return 1;

				continue;
			}
		}
	}

	return 0;
}
