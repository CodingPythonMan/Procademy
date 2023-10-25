#include <iostream>
#include <WS2tcpip.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE 512

int main()
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
}
