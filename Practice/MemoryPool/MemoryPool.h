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

	if (_Capacity <= 0)
	{
		_FreeNode = (Node*)malloc(sizeof(Node));
#ifdef _DEBUG
		if (_FreeNode == nullptr)
			return nullptr;

		_FreeNode->First = this;
		_FreeNode->Last = this;
#endif
		ptr = &_FreeNode->Data;
		new(ptr) T;
	}
	else
	{
		ptr = &_FreeNode->Data;
		// Placement New Ȱ��ȭ��� Alloc ���� ������ ȣ��.
		if (_PlacementNew == true)
		{
			new(ptr) T;
		}
		_FreeNode = _FreeNode->Next;
		_Capacity--;
	}
	
	_UseCount++;

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
	reinterpret_cast<Node*>pData->Next = _FreeNode;
	_FreeNode = reinterpret_cast<Node*>pData;
#endif
	_UseCount--;
	_Capacity++;

	return true;
}