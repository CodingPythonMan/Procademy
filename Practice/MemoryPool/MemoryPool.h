template <class T>
class MemoryPool
{
public:
	struct Node {
		T Data;
		Node* Prev;
	};

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
	// ���� ��� ����
	_FreeNode = (Node*)malloc(sizeof(Node));
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node;
		// PlacementNew �� Ȱ��ȭ �Ǿ��ִٸ� �����ڰ� ȣ����� �ʴ´�.
		if (PlacementNew == true)
		{
			node = (Node*)malloc(sizeof(Node));
		}
		else
		{
			node = new Node;
		}
		if(node == nullptr)
			return;

		node->Prev = _FreeNode;
		_FreeNode = node;
		//printf("[Node %d] : 0x%p\n", i, node);
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
	Node* node;

	// ���� Ǯ�� ��� ���� ���� �� �� ��찡 ȣ��ȴ�.
	ptr = &_FreeNode->Data;
	// ������ ȣ��
	if (_PlacementNew == true)
	{
		new(&_FreeNode->Data) T;
	}

	// Capacity �� ����� �� ���� �Ҵ����� �� �ִ�.
	if (_Capacity > 0)
	{
		_FreeNode = _FreeNode->Prev;
		_Capacity--;
	}
	else
	{
		node = (Node*)malloc(sizeof(Node));
		_FreeNode->Prev = node;
		_FreeNode = node;
	}
	
	_UseCount++;

	//printf("[Node Alloc] : 0x%p, Capacity : %d, UseCount : %d\n", ptr, _Capacity, _UseCount);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	if (pData != nullptr)
	{
		(*pData).~T();
		((Node*)pData)->Prev = _FreeNode;
		_FreeNode = (Node*)pData;
		_UseCount--;
		_Capacity++;
	}
	else
	{
		__debugbreak();
		return false;
	}

	//printf("[Node Free] : 0x%p, Capacity : %d, UseCount : %d\n", pData, _Capacity, _UseCount);

	return true;
}