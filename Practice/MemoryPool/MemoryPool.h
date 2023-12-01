template <class T>
class MemoryPool
{
public:
	struct Node {
		Node* UniqueValue;
		T Data;
		Node* FirstNode;
		Node* Prev;

		Node()
		{
			UniqueValue = nullptr;
			FirstNode = nullptr;
			Prev = nullptr;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 초기 블럭 개수.
	//				(bool) Alloc 시 생성자 / Free 시 파괴자 호출 여부
	// Return:
	//////////////////////////////////////////////////////////////////////////
	MemoryPool(int BlockNum = 0, bool PlacementNew = false);
	virtual	~MemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.  
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	T* Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(T* pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetCapacityCount(void) { return _Capacity; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _UseCount; }

private:
	int _Capacity;
	int _UseCount;
	bool _PlacementNew;
	// 스택 방식으로 반환된 (미사용) 오브젝트 블럭을 관리.
	Node* _FreeNode;
	Node* _PoolUniqueNode;
};

template<class T>
inline MemoryPool<T>::MemoryPool(int BlockNum, bool PlacementNew)
{
	// 더미 노드 설정
	if(PlacementNew == true)
		_FreeNode = (Node*)malloc(sizeof(Node));
	else
		_FreeNode = new Node;
	
	_PoolUniqueNode = _FreeNode;
	_Capacity = BlockNum;
	_PlacementNew = PlacementNew;
	_UseCount = 0;

	printf("Node Size : %d\n", (int)sizeof(Node));

	for (int i = 0; i < _Capacity; i++)
	{
		Node* node;
		// PlacementNew 가 활성화 되어있다면 생성자가 호출되지 않는다.
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
		printf("[Node %d] : 0x%p\n", i, node);
	}
}

template<class T>
inline MemoryPool<T>::~MemoryPool()
{
	delete _PoolUniqueNode;
}

template<class T>
inline T* MemoryPool<T>::Alloc(void)
{
	T* ptr = &_FreeNode->Data;
	
	// Placement New 활성화라면 Alloc 호출할 때, 생성자 호출
	if (_PlacementNew == true)
	{
		new(ptr) T;
	}

	// Capacity 가 충분할 때 쉽게 할당해줄 수 있다.
	if (_Capacity > 0)
	{
		_FreeNode = _FreeNode->Prev;
		_Capacity--;
	}
	else
	{
		// Placement New 라면 생성될 때, 생성자 호출되면 안 됨.
		if (_PlacementNew == true)  
			_FreeNode = (Node*)malloc(sizeof(Node));
		else
			_FreeNode = new Node;
	}
	
	_UseCount++;

	printf("[Node Alloc] : 0x%p, Capacity : %d, UseCount : %d\n", ptr-8, _Capacity, _UseCount);

	return ptr;
}

template<class T>
inline bool MemoryPool<T>::Free(T* pData)
{
	if (pData != nullptr)
	{
		pData->~T();
		reinterpret_cast<Node*>(pData - sizeof(Node*))->Prev = _FreeNode;
		_FreeNode = reinterpret_cast<Node*>(pData - sizeof(Node*));
		_UseCount--;
		_Capacity++;
	}
	else
	{
		__debugbreak();
		return false;
	}

	printf("[Node Free] : 0x%p, Capacity : %d, UseCount : %d\n", pData-8, _Capacity, _UseCount);

	return true;
}