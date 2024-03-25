#pragma once
#include <windows.h>
#include <new>

template <class T>
class MemoryPool
{
public:
#ifdef _DEBUG
#pragma pack(push, 1)
	struct Node {
		void* First;
		T Data;
		void* Last;
		Node* Next;
	};
#pragma pack(pop)
#else
	struct Node {
		T Data;
		Node* Next;
	};
#endif

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Parameters:	(int) �ʱ� �� ����.
	//				(bool) Alloc �� ������ / Free �� �ı��� ȣ�� ����
	// Return:
	//////////////////////////////////////////////////////////////////////////
	MemoryPool(int BlockNum = 0, bool PlacementNew = false);
	virtual	~MemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.  
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	T* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//
	// Parameters: (DATA *) �� ������.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(T* pData);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������ ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _Capacity; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _UseCount; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// ���� ������� ��ȯ�� (�̻��) ������Ʈ ���� ����.
	Node* _FreeNode;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	_FreeNode = nullptr;
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node = (Node*)malloc(sizeof(Node));
#ifdef _DEBUG
		if (node == nullptr)
			return;

		node->First = this;
		node->Last = this;
#endif
		// PlacementNew �� Ȱ��ȭ �� �Ǿ��ִٸ� ������ ȣ��
		if (PlacementNew == false)
		{
			new(&node->Data) T;
		}

		node->Next = _FreeNode;
		_FreeNode = node;
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

	Node* newFree = nullptr;
	Node* lastFree;

	if (_Capacity <= 0)
	{
		newFree = (Node*)malloc(sizeof(Node));

		if (newFree == nullptr)
			return nullptr;

#ifdef _DEBUG
		_FreeNode->First = this;
		_FreeNode->Last = this;
#endif
		ptr = &newFree->Data;
		new(ptr) T;

		do
		{
			lastFree = _FreeNode;
			newFree->Next = lastFree;
		} while (InterlockedCompareExchange64((LONG64*)&_FreeNode, (LONG64)newFree, (LONG64)lastFree) != (LONG64)lastFree);

		InterlockedIncrement((long*)&_UseCount);

		return ptr;
	}

	ptr = &_FreeNode->Data;
	// Placement New Ȱ��ȭ��� Alloc ���� ������ ȣ��.
	if (_PlacementNew == true)
	{
		new(ptr) T;
	}

	do
	{
		lastFree = _FreeNode;

		if (_FreeNode == nullptr)
			continue;

		newFree = _FreeNode->Next;
	} while (InterlockedCompareExchange64((LONG64*)&_FreeNode, (LONG64)newFree, (LONG64)lastFree) != (LONG64)lastFree);

	InterlockedIncrement((long*)&_UseCount);
	InterlockedDecrement((long*)&_Capacity);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
#ifdef _DEBUG
	if (pData == nullptr)
	{
		__debugbreak();
		return false;
	}
	Node* ptr = reinterpret_cast<Node*>((char*)pData - sizeof(MemoryPool*));

	if (ptr->First != this || ptr->Last != this)
		__debugbreak();
#endif

	pData->~T();

#ifdef _DEBUG
	ptr->Next = _FreeNode;
	_FreeNode = ptr;
#else

	Node* newFree = reinterpret_cast<Node*>(pData);
	Node* lastFree;

	do
	{
		lastFree = _FreeNode;
		newFree->Next = lastFree;
	} while (InterlockedCompareExchange64((LONG64*)&_FreeNode, (LONG64)newFree, (LONG64)lastFree) != (LONG64)lastFree);

#endif
	InterlockedDecrement((long*)&_UseCount);
	InterlockedIncrement((long*)&_Capacity);

	return true;
}