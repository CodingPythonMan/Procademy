#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <locale.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
	_wsetlocale(LC_ALL, L"korean");

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


}