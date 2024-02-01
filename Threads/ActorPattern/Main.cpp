#include "ActorPattern.h"
#include <conio.h>

#define RANDOM_MAX_LEN 30

wstring GenerateRanString(short len)
{
	wstring tempData;
	tempData.reserve(len);
	
	for (int i = 0; i < len; i++)
		tempData.push_back('a' + rand() % 26);

	return tempData;
}

int main()
{
	HANDLE WorkerThreads[WORKER_THREAD_NUMBER];
	for (int i = 0; i < WORKER_THREAD_NUMBER; i++)
	{
		WorkerThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, Work, nullptr, 0, nullptr);
	}

	while (1)
	{
		// L : ��Ʈ�� Lock / U : ��Ʈ�� Unlock / G : ���� ����
		if (_kbhit())
		{
			WCHAR ControlKey = _getwch();

			// Ű���� ���� Ǯ�� ���¿��� Ư�� ���
			if (L'q' == ControlKey || L'Q' == ControlKey)
			{
				wprintf(L"Control Mode : Press Q - Quit Send \n");
			}
		}

		// �� ���� ����
		short len = rand() % RANDOM_MAX_LEN;
		st_MSG_HEAD head;
		head.shType = rand() % dfJOB_QUIT;
		head.shPayloadLen = len * 2;
		wstring ranString = GenerateRanString(len);

		if (messageQ.GetFreeSize() >= head.shPayloadLen + sizeof(st_MSG_HEAD))
		{
			AcquireSRWLockExclusive(&lock);

			messageQ.Enqueue((char*)&head, sizeof(st_MSG_HEAD));
			messageQ.Enqueue((char*)&ranString[0], head.shPayloadLen);

			ReleaseSRWLockExclusive(&lock);
		}
		else
			__debugbreak();

		Sleep(50);
	}

	// ��Ŀ ������ ���� ���
	WaitForMultipleObjects(WORKER_THREAD_NUMBER, WorkerThreads, true, INFINITE);
}