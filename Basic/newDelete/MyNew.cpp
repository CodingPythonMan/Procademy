#include "MyNew.h"
#undef new

MyNew myNew;

MyNew::MyNew()
{
	for (int i = 0; i < MAX_ALLOC_INFO; i++)
	{
		memset(allocInfos, 0, sizeof(AllocInfo) * MAX_ALLOC_INFO);
	}
	for (int i = 0; i < 100; i++)
	{
		memset(logs + i, 0, 100);
	}
	totalAlloc = 0;
	totalLog = 0;
}

MyNew::~MyNew()
{
	// Leak üũ
	for (int i = 0; i < totalAlloc; i++)
	{
		if (allocInfos[i].Ptr == 0)
			break;

		myNew.AddLog(MyNewError::LEAK, allocInfos[i].Ptr, i);
	}

	// Log ���� �ۼ�
	char fileName[70];
	tm t;
	time_t timer;

	timer = time(NULL);    // ���� �ð��� �� ������ ���
	localtime_s(&t, &timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�

	sprintf_s(fileName, 70, "Alloc_%04d%02d%02d_%02d%02d%02d.txt",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
		t.tm_hour, t.tm_min, t.tm_sec
	);

	FILE* file;

	fopen_s(&file, fileName,"w");
	if (file == nullptr)
		return;

	for (int i = 0; i < totalLog; i++)
	{
		fwrite(logs[i], 1, strlen(logs[i]), file);
	}
	
	fclose(file);
}

void MyNew::AddInfo(void* Ptr, int Size, char FileName[64], int Line, bool Array)
{
	// new ����� �� �ִ� �ִ� ���� �ʰ�
	if (totalAlloc >= MAX_ALLOC_INFO-1)
	{
		printf("New Delete ������ �ʰ� : �ִ�=%d\n", totalAlloc);
		return;
	}

	allocInfos[totalAlloc].Ptr = Ptr;
	allocInfos[totalAlloc].Size = Size;
	memcpy(allocInfos[totalAlloc].FileName, FileName, strlen(FileName));
	allocInfos[totalAlloc].Line = Line;
	allocInfos[totalAlloc].Array = Array;

	totalAlloc++;
}

void MyNew::AddLog(MyNewError error, void* ptr, int index)
{
	// new ����� �� �ִ� �ִ� ���� �ʰ�
	if (totalLog >= MAX_ALLOC_INFO-1)
	{
		printf("New Delete �α� ������ �ʰ� : �ִ�=%d\n", totalLog);
		return;
	}

	switch (error)
	{
	case MyNewError::NOALLOC:
	{
		sprintf_s(logs[totalLog], "NOALLOC [0x%p]\n", ptr);
		break;
	}
	case MyNewError::ARRAY:
	{
		sprintf_s(logs[totalLog], "ARRAY [0x%p] [%8d] %s : %d\n",
			ptr, allocInfos[index].Size, allocInfos[index].FileName, allocInfos[index].Line);
		break;
	}
	case MyNewError::LEAK:
	{
		sprintf_s(logs[totalLog], "LEAK [0x%p] [%8d] %s : %d\n",
			ptr, allocInfos[index].Size, allocInfos[index].FileName, allocInfos[index].Line);
		break;
	}
	default:
		return;
	}

	totalLog++;
}

void MyNew::ResetInfoIndex(int index)
{
	for (int i = index; i < totalAlloc; i++)
		allocInfos[index] = allocInfos[index + 1];

	totalAlloc--;
}

// �� ����� new ȣ�� ���
void* operator new(size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);

	myNew.AddInfo(ptr, (int)size, (char*)File, Line, false);

	return ptr;
}

void* operator new[](size_t size, const char* File, int Line)
{
	void* ptr = malloc(size);

	myNew.AddInfo(ptr, (int)size, (char*)File, Line, true);

	return ptr;
}

void operator delete (void* p, const char* File, int Line)
{
}
void operator delete[](void* p, const char* File, int Line)
{
}

//������ ����� delete
void operator delete(void* p)
{
	// ���� malloc���� �Ҵ���� �迭 p �� ���޹��� p �� ��ġ�� �ٸ���.
	void* ptr = (void*)((__int64)p - sizeof(void*));

	for (int i = 0; i < myNew.totalAlloc; i++)
	{
		// �Ҵ�� ���� ����
		if (myNew.allocInfos[i].Ptr == p && myNew.allocInfos[i].Array == false)
		{
			free(p);
			myNew.ResetInfoIndex(i);
			return;
		}
		else if (myNew.allocInfos[i].Ptr == ptr && myNew.allocInfos[i].Array == true)
		{
			myNew.AddLog(MyNewError::ARRAY, p, i);
			free(ptr);
			myNew.ResetInfoIndex(i);
			return;
		}
	}

	// �Ҵ���� ���� ������ ����
	myNew.AddLog(MyNewError::NOALLOC, p);
}

void operator delete[](void* p)
{
	for (int i = 0; i < myNew.totalAlloc; i++)
	{
		// �Ҵ�� ���� ����
		if (myNew.allocInfos[i].Ptr == p && myNew.allocInfos[i].Array == true)
		{
			free(p);
			myNew.ResetInfoIndex(i);
			return;
		}
	}

	// �Ҵ���� ���� ������ ����
	myNew.AddLog(MyNewError::NOALLOC, p);
}