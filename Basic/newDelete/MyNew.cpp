#include "MyNew.h"
#undef new

MyNew myNew;

MyNew::MyNew()
{
	printf("����!\n");
}

MyNew::~MyNew()
{
	printf("����!\n");
}

// �� ����� new ȣ�� ���
void* operator new(size_t size, const char* File, int Line)
{
	//void* ptr = malloc(size);

	void* ptr = nullptr;

	printf("ahaha\n");
	printf("File: %s Line: %d\n", File, Line);

	return ptr;
}

void* operator new[](size_t size, char* File, int Line)
{
	void* ptr = malloc(size);

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

}

void operator delete[](void* p)
{

}