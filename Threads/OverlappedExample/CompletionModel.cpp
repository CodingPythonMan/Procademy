#include "CompletionModel.h"

int IocpMain()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ����� �Ϸ� ��Ʈ ����
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return 1;

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// (CPU ���� * 2)���� �۾��� ������ ����
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread003, hcp, 0, nullptr);
		if (hThread == NULL)
			return 1;
		CloseHandle(hThread);
	}

	// socket()
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return 1;
	
	// bind()
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 1;

	// listen ���� �۽Ź��� 0���� ����
	/*
	int optval = 0;
	setsockopt(listenSock, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));

	int getoptval = 100;
	int optlen = sizeof(getoptval);
	int result = getsockopt(listenSock, SOL_SOCKET, SO_SNDBUF, (char*)&getoptval, &optlen);
	if (result == SOCKET_ERROR)
	{
		result = WSAGetLastError();

		std::cout << "ErrorCode : " << result;
	}
	else
	{
		std::cout << "Result : " << result << " optval : " << getoptval;
	}*/
	
	// listen()
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return 1;

	// ������ ��ſ� ����� ����
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	DWORD recvbytes, flags;
	WCHAR IP[16];

	while (1)
	{
		// accept()
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
			break;

		InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
		wprintf(L"[TCP Server] Client Access : IP Address=%s, Port=%d\n",
			IP, ntohs(clientAddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)clientSock, hcp, clientSock, 0 );

		// ���� ���� ����ü �Ҵ�
		SOCKETINFO* ptr = new SOCKETINFO;
		if (ptr == nullptr)
			break;

		memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
		ptr->sock = clientSock;
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		// �񵿱� ����� ����
		flags = 0;
		retval = WSARecv(clientSock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, nullptr);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				return -1;
			}
			continue;
		}
	}

	// ���� ����
	WSACleanup();
	return 0;
}

unsigned int WINAPI WorkerThread003(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	WCHAR IP[16];

	while (1)
	{
		// �񵿱� ����� �Ϸ� ��ٸ���
		DWORD Transferred;
		SOCKET clientSock;
		SOCKETINFO* ptr;
		retval = GetQueuedCompletionStatus(hcp, &Transferred,
			&clientSock, (LPOVERLAPPED*)&ptr, INFINITE);

		// Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);

		// �񵿱� ����� ��� Ȯ��
		if (retval == 0 || Transferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
			}
			closesocket(ptr->sock);
			InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
			wprintf(L"[TCP Server] Client Terminate : IP Address=%s, Port=%d\n",
				IP, ntohs(clientAddr.sin_port));
			delete ptr;
			continue;
		}

		// ������ ���۷� ����
		if (ptr->recvbytes == 0)
		{
			ptr->recvbytes = Transferred;
			ptr->sendbytes = 0;
			// ���� ������ ���
			ptr->buf[ptr->recvbytes] = '\0';

			InetNtop(AF_INET, &(clientAddr.sin_addr), IP, 16);
			wprintf(L"[TCP/%s:%d] ", IP, ntohs(clientAddr.sin_port));
			printf("%s\n", ptr->buf);
		}
		else
		{
			ptr->sendbytes += Transferred;
		}

		if (ptr->recvbytes > ptr->sendbytes)
		{
			// ������ ������
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1,
				&sendbytes, 0, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					return 1;
				}
				continue;
			}
		}
		else
		{
			ptr->recvbytes = 0;

			// ������ �ޱ�
			memset(&ptr->overlapped, 0, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			DWORD flags = 0;
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1,
				&recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					return 1;
				}
				continue;
			}
		}
	}

	return 0;
}