#pragma once
#include <windows.h>
#include <new>

#define NOT_TAGGING_BIT 48

template <class T>
class MemoryPool
{
public:
	struct Node {
		T Data;
		Node* Next;
	};

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//////////////////////////////////////////////////////////////////////////
	MemoryPool(int BlockNum = 0, bool PlacementNew = false);
	virtual	~MemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.  
	//////////////////////////////////////////////////////////////////////////
	T* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(T* pData);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _Capacity; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _UseCount; }

	//////////////////////////////////////////////////////////////////////////
	// ABA ���� �ذ�� Counter ����
	//////////////////////////////////////////////////////////////////////////
	LONG64 GetCounter(LONG64 ptr) { return  static_cast<unsigned __int64>(ptr) >> NOT_TAGGING_BIT; }

	//////////////////////////////////////////////////////////////////////////
	// ABA ���� �ذ�� Counter ����
	//////////////////////////////////////////////////////////////////////////
	LONG64 SetCounter(LONG64 counter) { return  counter << NOT_TAGGING_BIT; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// ���� ������� ��ȯ�� (�̻��) ������Ʈ ���� ����.
	LONG64 _FreeNode;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	_FreeNode = 0;
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node = (Node*)malloc(sizeof(Node));
		
		if (node == nullptr)
			return;

		// PlacementNew �� Ȱ��ȭ �� �Ǿ��ִٸ� ������ ȣ��
		if (PlacementNew == false)
		{
			new(&node->Data) T;
		}

		// ���� ó������ �±��ص� �ʿ�� ����.
		// CAS �� �Ͼ�� �������� �±��� �ʿ��ϴ�.
		node->Next = reinterpret_cast<Node*>(_FreeNode);
		_FreeNode = reinterpret_cast<LONG64>(node);
	}
}

template<class T>
inline MemoryPool<T>::~MemoryPool()
{
	
}

template<class T>
inline T* MemoryPool<T>::Alloc(void)
{
	T* ptr;

	LONG64 newFree;
	LONG64 lastFree;

	LONG64 ptrValue = _FreeNode - SetCounter(GetCounter(_FreeNode));
	ptr = &reinterpret_cast<Node*>(ptrValue)->Data;

	// Placement New Ȱ��ȭ��� Alloc ���� ������ ȣ��.
	if (ptrValue != NULL && _PlacementNew == true)
	{
		new(reinterpret_cast<T*>(ptrValue)) T;
	}

	do
	{
		lastFree = _FreeNode;

		if (lastFree == NULL)
		{
			Node* newNode = (Node*)malloc(sizeof(Node));

			if (newNode == nullptr)
				return nullptr;

			ptr = &newNode->Data;
			new(ptr) T;

			InterlockedIncrement((long*)&_UseCount);

			return ptr;
		}

		Node* newTop = reinterpret_cast<Node*>(lastFree - SetCounter(GetCounter(lastFree)));
		newFree = reinterpret_cast<LONG64>(newTop->Next);
	} 
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedIncrement((long*)&_UseCount);
	InterlockedDecrement((long*)&_Capacity);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	LONG64 newFree;
	LONG64 lastFree;
	unsigned __int64 Counter;

	newFree = reinterpret_cast<LONG64>(pData);
	LONG64 ptr = newFree - SetCounter(GetCounter(newFree));
	reinterpret_cast<T*>(ptr)->~T();

	do 
	{
		lastFree = _FreeNode;
		Counter = GetCounter(lastFree) + 1;
		reinterpret_cast<Node*>(ptr)->Next = reinterpret_cast<Node*>(lastFree);
		// new Free �� �±�
		newFree = ptr + SetCounter(Counter);
	} 
	while (InterlockedCompareExchange64(&_FreeNode, newFree, lastFree) != lastFree);

	InterlockedDecrement((long*)&_UseCount);
	InterlockedIncrement((long*)&_Capacity);

	return true;
}