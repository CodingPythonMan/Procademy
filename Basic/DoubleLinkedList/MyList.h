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
		iterator(Node* node = nullptr) : _Node(nullptr)
		{

		}

		iterator operator++(int)
		{
			// ���� ��带 ���� ���� �̵�

		}

		iterator& operator++()
		{
			return *this;
		}

		iterator operator--(int)
		{

		}

		iterator& operator--()
		{
			return *this;
		}

		T& operator*()
		{
			// ���� ����� ������ ����
		}
		bool operator==(const iterator& other)
		{

		}
		bool operator!=(const iterator& other)
		{

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
	}

	iterator end()
	{
		// Tail ��带 ����Ű�� (�����Ͱ� ���� ��¥ ���� �� ���) ���ͷ����͸� ����
		// �Ǵ� ������ ������ �� �ִ� ���ͷ����͸� ����
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
		return _size;
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