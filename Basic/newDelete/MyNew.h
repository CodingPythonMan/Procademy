#pragma once
#include <stdlib.h>
#include <stdio.h>

class MyNew {

	struct AllocInfo
	{
		void* Ptr;
		int Size;
		char FileName[64];
		int Line;
		bool Array;
	};

	MyNew();
	virtual ~MyNew();

	void* operator new(size_t size, const char* File = __FILE__, int Line = __LINE__)
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


	//������ ����� delete
	void operator delete(void* p)
	{

	}

	void operator delete[](void* p)
	{

	}

		// �� ����� new ȣ�� ���
};


#define new MyNew::new(__FILE__, __LINE__)