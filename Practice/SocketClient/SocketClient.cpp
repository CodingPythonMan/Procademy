#include "SetClients.h"
/*#include <cstdio>
#include <WS2tcpip.h>
#include <windows.h>

#define FILE_DATA 100000

struct st_PACKET_HEADER
{
	DWORD	dwPacketCode;		// 0x11223344	�츮�� ��ŶȮ�� ������

	WCHAR	szName[32];		// �����̸�, �����ڵ� NULL ���� ��
	WCHAR	szFileName[128];	// �����̸�, �����ڵ� NULL ���� ��
	int	iFileSize;
};*/

int main()
{
	SetClients();
	/*
	// ��Ŷ ��� �غ�
	st_PACKET_HEADER packetHeader;
	packetHeader.dwPacketCode = 0x11223344;
	wcscpy_s(packetHeader.szName, L"JuYoungKim");
	packetHeader.szName[wcslen(packetHeader.szName)] = '\0';
	wcscpy_s(packetHeader.szFileName, L"nanachi.jpg");
	packetHeader.szFileName[wcslen(packetHeader.szFileName)] = '\0';

	// ���� ������ �غ�
	FILE* file;
	fopen_s(&file, "nanachi.jpg", "rb");

	if (file == nullptr)
		return -1;

	char* fileData = (char*)malloc(FILE_DATA);
	if (fileData == 0)
		return -1;

	fread(fileData, FILE_DATA, 1, file);

	fseek(file, 0L, SEEK_END);
	int size = (int)ftell(file);

	fseek(file, 0L, SEEK_SET);

	packetHeader.iFileSize = size;

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
	InetPton(AF_INET, L"106.245.38.108", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(10010);
	retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (retval == SOCKET_ERROR)
		return 0;

	// ��� ������ ����
	retval = send(sock, (char*)&packetHeader, sizeof(st_PACKET_HEADER), 0);
	if (retval == SOCKET_ERROR)
		return -1;

	// ���� ������ ����
	retval = send(sock, fileData, size, 0);
	if (retval == SOCKET_ERROR)
		return -1;

	char buf[8];
	// ������ �ޱ�
	retval = recv(sock, buf, 8, 0);
	if (retval == SOCKET_ERROR)
		return -1;
	else if (retval == 0)
		return -1;

	free(fileData);
	fclose(file);

	// close
	closesocket(sock);

	WSACleanup();

	return 0;*/
}