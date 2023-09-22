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
	};

public:
	MyList()
	{

	}
	~MyList()
	{

	}

	iterator begin()
	{
		// ù��° ������ ��带 ����Ű�� ���ͷ����� ����
		iterator it(_Head);
		return it;
	}

	iterator end()
	{
		// Tail ��带 ����Ű�� (�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
		// �Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����
		iterator it(_Tail);
		return it;
	}

	void push_front(T data)
	{
		Node node;
		node._Data = data;

	}

	void push_back(T data)
	{

	}

	void pop_front()
	{

	}

	void pop_back()
	{

	}

	void clear()
	{

	}

	int size()
	{
		return _Size;
	}

	bool empty()
	{

	}

	iterator erase(iterator iter)
	{
		// ���ͷ������� �� ��带 ����.
		// �׸��� ���� ����� ���� ��带 ����Ű�� ���ͷ����� ����
	}

	void remove(T data)
	{

	}

private:
	int _Size = 0;
	Node* _Head;
	Node* _Tail;
};