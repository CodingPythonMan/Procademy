#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32")

#define SERVERPORT 1902
#define BUFSIZE 512

struct SOCKETINFO
{
    SOCKET sock;
    char buf[BUFSIZE + 1];
};

int TotalSockets = 0;
SOCKETINFO* SocketInfoArr[FD_SETSIZE];

// ���� ���� �߰�
BOOL AddSocketInfo(SOCKET sock)
{
    if (TotalSockets >= FD_SETSIZE)
    {
        printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
        return FALSE;
    }

    SOCKETINFO* ptr = new SOCKETINFO;
    if (ptr == nullptr)
    {
        printf("[����] �޸𸮰� �����մϴ�!\n");
        return FALSE;
    }

    ptr->sock = sock;
    SocketInfoArr[TotalSockets] = ptr;
    TotalSockets++;

    return TRUE;
}

// ���� ���� ����
void RemoveSocketInfo(int index)
{
    SOCKETINFO* ptr = SocketInfoArr[index];

    // Ŭ���̾�Ʈ ���� ���
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);
    WCHAR clientIP[16] = { 0 };
	InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
	wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n", clientIP, ntohs(clientAddr.sin_port));

    closesocket(ptr->sock);
    delete ptr;

    if (index != (TotalSockets - 1))
        SocketInfoArr[index] = SocketInfoArr[TotalSockets - 1];

    TotalSockets--;
}

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
    FD_SET rset;
    SOCKET clientSock;
    SOCKADDR_IN clientAddr;
    int addrLen;
    WCHAR clientIP[16] = {0};

    while (1)
    {
        // ���� �� �ʱ�ȭ
        FD_ZERO(&rset);
        FD_SET(listenSock, &rset);
        for (int i = 0; i < TotalSockets; i++)
        {
            FD_SET(SocketInfoArr[i]->sock, &rset);
        }

        // select()
        retval = select(0, &rset, nullptr, nullptr, nullptr);
        if (retval == SOCKET_ERROR)
            return 1;

        // ���� �� �˻�(1) : Ŭ���̾�Ʈ ���� ����
        if (FD_ISSET(listenSock, &rset)) {
            addrLen = sizeof(clientAddr);
            clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
            if (clientSock == INVALID_SOCKET)
                return 1;
            else
            {
                InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
                wprintf(L"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n", clientIP, ntohs(clientAddr.sin_port));

                // ���� ���� �߰�
                AddSocketInfo(clientSock);
            }
        }

        // ���� �� �˻�(2) : ������ ���
        for (int i = 0; i < TotalSockets; i++)
        {
            SOCKETINFO* ptr = SocketInfoArr[i];
            if (FD_ISSET(ptr->sock, &rset))
            {
                // ������ �ޱ�
                retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
                if (retval == SOCKET_ERROR) {
                    RemoveSocketInfo(i);
                    continue;
                }
                else if(retval == 0)
                {
                    RemoveSocketInfo(i);
                    continue;
                }

                // ���� ������ ���
                addrLen = sizeof(clientAddr);
                getpeername(ptr->sock, (SOCKADDR*)&clientAddr, &addrLen);
                ptr->buf[retval] = '\0';
                InetNtop(AF_INET, &clientAddr.sin_addr, clientIP, 16);
                wprintf(L"[TCP/%s:%d]", clientIP, ntohs(clientAddr.sin_port));
                printf(" %s\n", ptr->buf);

                // �츰 write_set �� �� ���Ƿ� ���⼭ Send �ؾ��Ѵ�.
                retval = send(ptr->sock, ptr->buf, retval, 0);
                if (retval == SOCKET_ERROR)
                {
                    RemoveSocketInfo(i);
                    return;
                }
            }
        }
    }

    // ���� ����
    WSACleanup();
    return 0;
}