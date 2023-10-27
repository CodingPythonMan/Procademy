#include "SetClients.h"

#define BUFSIZE 512

int SetDefaultEchoClient()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return 0;

	// connect
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(1902);
	retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 0;

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	size_t len;

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n\ ���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// ������ ������
		retval = send(sock, buf, (int)strlen(buf), 0);
		if (retval == SOCKET_ERROR)
			break;

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		retval = recv(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
			break;
		else if (retval == 0)
			break;

		// ���� ������ ���
		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�. \n", retval);
		printf("[���� ������] %s\n", buf);
	}

	// close
	closesocket(sock);

	WSACleanup();

	return 0;
}

void SetClients()
{
	int index = 0;
	SOCKET* clients = new SOCKET[50000];
	while (true)
	{
		clients[index] = socket(AF_INET, SOCK_STREAM, 0);
		if (clients[index] == INVALID_SOCKET)
			return;

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(14444);
		int retval = connect(clients[index], (sockaddr*)&addr, sizeof(addr));
		if (retval == SOCKET_ERROR)
			break;

		index++;

	}
	printf("%d client connect", index);
	delete[] clients;

	printf("WSAGetLastError : %d", WSAGetLastError());
}

void SetCloseClient()
{
	int index = 0;
	SOCKET client;
	while (true)
	{
		client = socket(AF_INET, SOCK_STREAM, 0);
		if (client == INVALID_SOCKET)
			return;

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(14444);
		int retval = connect(client, (sockaddr*)&addr, sizeof(addr));
		if (retval == SOCKET_ERROR)
			break;

		index++;

		closesocket(client);
	}
	printf("%d client connect", index);

	printf("WSAGetLastError : %d", WSAGetLastError());
}
