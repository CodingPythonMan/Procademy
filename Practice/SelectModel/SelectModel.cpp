#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32")

#define SERVERPORT 1902
#define BUFSIZE 512

// ���� ���� ������ ���� ����ü�� ����
int main()
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket
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

    // ����ŷ �������� ��ȯ
    u_long on = 1;
    retval = ioctlsocket(listenSock, FIONBIO, &on);
    if (retval == SOCKET_ERROR)
        return 1;

    // ������ ��ſ� ����� ����
    FD_SET rset, wset;

}