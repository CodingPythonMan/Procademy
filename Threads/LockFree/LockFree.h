#pragma once
#include <windows.h>

template<typename T>
class LockFree
{
	struct Node {
		Node* Next;
		T Data;
		//int arr[100000000] = { 0 };
	};

public:
	void Push(T& data)
	{
		Node* newNode = new Node();
		newNode->Data = data;

		Node* lastTop;
		do 
		{
			lastTop = _top;
			newNode->Next = lastTop;
		} 
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newNode, (LONG64)lastTop) != (LONG64)lastTop);
	}

	void Pop(void)
	{
		Node* lastTop;
		Node* newTop;
		do
		{
			lastTop = _top;
			newTop = _top->Next;
		}
		while (InterlockedCompareExchange64((LONG64*)&_top, (LONG64)newTop, (LONG64)lastTop) != (LONG64)lastTop);

		delete lastTop;
	}

private:
	Node* _top;
};

