#pragma once

template<typename T>
class MyList
{
public:
	struct Node
	{
		T _Data;
		Node* _Prev;
		Node* _Next;
	};

	class iterator
	{
	private:
		Node* _Node;
	public:
		iterator(Node* node = nullptr)
		{
			_Node = node;
		}

		iterator operator++(int)
		{
			iterator it(_Node);
			// ���� ��带 ���� ���� �̵�
			_Node = _Node->_Next;
			return it;
		}

		iterator& operator++()
		{
			_Node = _Node->_Next;
			return *this;
		}

		iterator operator--(int)
		{
			iterator it(_Node);
			_Node = _Node->_Prev;
			return it;
		}

		iterator& operator--()
		{
			_Node = _Node->_Prev;
			return *this;
		}

		T& operator*()
		{
			// ���� ����� ������ ����
			return _Node->_Data;
		}
		bool operator==(const iterator& other)
		{
			if (_Node == other._Node)
				return true;
			return false;
		}
		bool operator!=(const iterator& other)
		{
			if (_Node == other._Node)
				return false;
			return true;
		}

		friend MyList;
	};

public:
	MyList()
	{
		_Head._Next = &_Tail;
		_Tail._Prev = &_Head;
	}
	~MyList()
	{
	}

	iterator begin()
	{
		// ù��° ������ ��带 ����Ű�� ���ͷ����� ����
		iterator it(_Head._Next);
		return it;
	}

	iterator end()
	{
		// Tail ��带 ����Ű�� (�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
		// �Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����
		iterator it(&_Tail);
		return it;
	}

	void push_front(T data)
	{
		Node* node = new Node();
		node->_Data = data;

		_Head._Next->_Prev = node;
		node->_Next = _Head._Next;
		node->_Prev = &_Head;
		_Head._Next = node;

		_Size++;
	}

	void push_back(T data)
	{
		Node* node = new Node();
		node->_Data = data;

		_Tail._Prev->_Next = node;
		node->_Prev = _Tail._Prev;
		node->_Next = &_Tail;
		_Tail._Prev = node;

		_Size++;
	}

	void pop_front()
	{
		Node* node = _Head._Next;
		_Head._Next = node->_Next;
		node->_Next->_Prev = node->_Prev;

		delete(node);

		_Size--;
	}

	void pop_back()
	{
		Node* node = _Tail._Prev;
		_Tail._Prev = node->_Prev;
		node->_Prev->_Next = node->_Next;

		delete(node);

		_Size--;
	}

	void clear()
	{
		Node* node = _Head._Next;
		_Head._Next = &_Tail;
		_Tail._Prev = &_Head;

		while (node != &_Tail)
		{
			Node* nextNode = node->_Next;
			delete(node);
			node = nextNode;
			_Size--;
		}
	}

	int size()
	{
		return _Size;
	}

	bool empty()
	{
		if (_Size <= 0)
			return true;
		else
			return false;
	}

	iterator erase(iterator iter)
	{
		// ���ͷ������� �� ��带 ����.
		// �׸��� ���� ����� ���� ��带 ����Ű�� ���ͷ����� ����
		Node* node = iter._Node;
		iterator newIter(node);
		++newIter;

		node->_Prev->_Next = node->_Next;
		node->_Next->_Prev = node->_Prev;

		delete(node);
		return newIter;
	}

	void remove(T data)
	{
		if (_Size > 0)
		{
			for (iterator iter = begin(); iter != end();)
			{
				if (*iter == data)
				{
					iter = erase(iter);
				}
				else
				{
					++iter;
				}
			}
		}
	}

private:
	int _Size = 0;
	Node _Head;
	Node _Tail;
};