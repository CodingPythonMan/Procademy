#include "StarClient.h"

int main()
{
	ConsoleInit();

	printf("������ IP �ּҸ� �Է��ϼ��� : ");
	WCHAR IP[16];
	wscanf_s(L"%s", IP, 16);
	IP[wcslen(IP)] = '\0';

	if (SetClientSocket())
		return 0;

	if (ConnectClientSocket(IP))
		return 0;

	if (SelectLoop())
		return 0;

	if (EndSocket())
		return 0;
}