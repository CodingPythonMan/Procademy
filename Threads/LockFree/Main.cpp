#include "LockFree.h"
#include <process.h>
#include <windows.h>

LockFree<int> lockFree;

unsigned int WorkerThread(LPVOID lpParam)
{
	while (1)
	{
		for (int i=0; i<1000; i++)
			lockFree.Push(i);

		for (int i=0; i<1000; i++)
			lockFree.Pop();
	}

	return 0;
}

int main()
{
	HANDLE Threads[6];

	for (int i = 0; i < 6; i++)
	{
		Threads[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
	}

	WaitForMultipleObjects(6, Threads, true, INFINITE);
}