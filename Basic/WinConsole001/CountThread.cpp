#include <stdio.h>
#include <tchar.h>
#include <windows.h>

// CountThread.cpp
#define MAX_THREADS (1024*10)

DWORD WINAPI ThreadProcCount(LPVOID lpParam)
{
	DWORD threadNum = (DWORD)lpParam;

	while (1)
	{
		wprintf(L"thread num : %d \n", threadNum);
		Sleep(5000);
	}

	return 0;
}

DWORD countOfThread = 0;

int CountMaxThreads(int argc, WCHAR* argv[])
{
	DWORD threadID[MAX_THREADS];
	HANDLE hThread[MAX_THREADS];

	// ���� ������ �ִ� ������ ������ ����
	while (1)
	{
		hThread[countOfThread] =
			CreateThread(
				nullptr, // ����Ʈ ���� �Ӽ� ����
				10, // ����Ʈ ���� ������
				ThreadProcCount, // ������ �Լ�
				(LPVOID)countOfThread, // ������ �Լ� ��������
				0,
				&threadID[countOfThread] // ������ ID ��ȯ
			);
		// ������ ���� Ȯ��
		if (hThread[countOfThread] == nullptr)
		{
			wprintf(L"MAXIMUM THREAD NUMBER: %d \n", countOfThread);
			break;
		}
		countOfThread++;
	}

	for (DWORD i = 0; i < countOfThread; i++)
	{
		CloseHandle(hThread[i]);
	}

	return 0;
}