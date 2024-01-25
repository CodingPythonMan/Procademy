#include <iostream>
#include <process.h>
#include <windows.h>

bool flag[2] = { false, false };
int turn = 0;

int Items = 0;
long lock = 0;

unsigned int WINAPI Thread001(LPVOID lpParam)
{
	for (int i = 0; i < 100000000; i++)
	{
		flag[0] = true;
		turn = 0;
		while (1)
		{
			if (flag[1] != true || turn == 1)
			{
				long a = InterlockedExchange(&lock, 1);
				if (a == 1)
					printf("Thread1 break => Flag[0] : %d, Flag[1] : %d, turn : %d, Items: %d\n", flag[0], flag[1], turn, Items);
				break;
			}
		}

		Items++;

		lock = 0;
		flag[0] = false;
	}

	return 0;
}

unsigned int WINAPI Thread002(LPVOID lpParam)
{
	for (int i = 0; i < 100000000; i++)
	{
		flag[1] = true;
		turn = 1;
		while (1)
		{
			if (flag[0] != true || turn == 0)
			{
				long a = InterlockedExchange(&lock, 1);
				if (a == 1)
					printf("Thread2 break => Flag[0] : %d, Flag[1] : %d, turn : %d, Items: %d\n", flag[0], flag[1], turn, Items);
				break;
			}
		}

		Items++;

		lock = 0;
		flag[1] = false;
	}

	return 0;
}

int main()
{

	HANDLE threads[2];
	threads[0] = (HANDLE*)_beginthreadex(nullptr, 0, Thread001, nullptr, 0, nullptr);
	threads[1] = (HANDLE*)_beginthreadex(nullptr, 0, Thread002, nullptr, 0, nullptr);

	WaitForMultipleObjects(2, threads, true, INFINITE);
	//WaitForSingleObject(threads[1], INFINITE);

	printf("Items : %d\n", Items);
}