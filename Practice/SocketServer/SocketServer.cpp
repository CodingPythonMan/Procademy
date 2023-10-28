#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <locale.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 512

int main()
{
	_wsetlocale(LC_ALL, L"korean");

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return 0;

	// Bind
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(1902);
	int retval = bind(listenSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 0;

	// Listen
	retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return 0;

	// ������ ��ſ� ����� ����
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	char buf[BUFSIZE + 1];
	char responseBuf[4];
	WCHAR clientIP[16] = { 0 };

	// ���� ���� 0xdd dd dd dd �� ä���
	for (int i = 0; i < 4; i++)
	{
		responseBuf[i] = '0xdd';
	}

	while (true)
	{
		// accept()
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET)
			break;

		// ������ Ŭ���̾�Ʈ ���� ���
		InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
		wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n", clientIP, ntohs(clientAddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			// ������ �ޱ�
			retval = recv(clientSock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR)
				break;
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			wprintf(L"[TCP/%s:%d] ", clientIP, ntohs(clientAddr.sin_port));
			printf(" %s\n", buf);

			// ������ ������
			retval = send(clientSock, buf, retval, 0);
			if (retval == SOCKET_ERROR)
				break;
		}

		// close
		closesocket(clientSock);
		wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n", clientIP, ntohs(clientAddr.sin_port));
	}

	closesocket(listenSock);

	WSACleanup();

	return 0;
}