#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

int gData = 0;
int gConnect = 0;
bool gShutdown = false;

CRITICAL_SECTION cs;

unsigned int WINAPI AcceptThread(LPVOID lpParam)
{
	int random;

	while (gShutdown == false)
	{
		random = 10 + (rand() % 991);
		InterlockedIncrement((long*)&gConnect);
		Sleep(random);
	}

	return 0;
}

unsigned int WINAPI DisconnectThread(LPVOID lpParam)
{
	int random;

	while (gShutdown == false)
	{
		random = 10 + (rand() % 991);
		InterlockedDecrement((long*)&gConnect);
		Sleep(random);
	}

	return 0;
}

unsigned int WINAPI UpdateThread(LPVOID lpParam)
{
	while (gShutdown == false)
	{
		EnterCriticalSection(&cs);

		gData++;
		if (gData % 1000 == 0)
			printf("gData : %d\n", gData);

		LeaveCriticalSection(&cs);

		Sleep(10);
	}

	return 0;
}

int main()
{
	timeBeginPeriod(1);

	HANDLE hThreads[5];
	hThreads[0] = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, 0, 0, nullptr);

	hThreads[1] = (HANDLE)_beginthreadex(nullptr, 0, DisconnectThread, 0, 0, nullptr);

	InitializeCriticalSection(&cs);
	hThreads[2] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);
	hThreads[3] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);
	hThreads[4] = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, 0, 0, nullptr);

	for(int i=0; i<20; i++)
	{
		printf("gConnect : %d\n", gConnect);
		Sleep(1000);
	}
	gShutdown = true;

	// ������ ���� Ȯ���� �ؾ��Ѵ�. �̰� ��� ���� �����غ���.
	// ���ο��� �������� ���� Ȯ���� WaitForMultipleObjects ���.
	WaitForMultipleObjects(5, hThreads, true, INFINITE);

	DeleteCriticalSection(&cs);

	return 0;
}