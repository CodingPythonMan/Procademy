#include "BinaryTree.h"
#include <iostream>

BinaryTree::BinaryTree()
{
	_Root = nullptr;
}

BinaryTree::~BinaryTree()
{
	DeleteDestructor(_Root);
}

bool BinaryTree::Insert(int Data)
{
	if (_Root == nullptr)
	{
		_Root = new Node;
		_Root->Data = Data;
		return true;
	}

	Node* node = _Root;
	while (1)
	{
		if (Data < node->Data)
		{
			if (node->Left == nullptr)
			{		
				Node* newNode = new Node;
				newNode->Data = Data;
				node->Left = newNode;
				break;
			}
			else
			{
				node = node->Left;
			}
		}
		else if (Data == node->Data)
		{
			return false;
		}
		else
		{
			if (node->Right == nullptr)
			{
				Node* newNode = new Node;
				newNode->Data = Data;
				node->Right = newNode;
				break;
			}
			else
			{
				node = node->Right;
			}
		}
	}

	return true;
}

bool BinaryTree::Delete(int Data)
{
	// ���� �ΰ� ������ ������ Ȯ�� ����
	return Delete(_Root, nullptr, Data);
}

bool BinaryTree::Find(int Data)
{
	return Find(_Root, Data);
}

bool BinaryTree::Delete(Node* node, Node* Parent, int Data)
{
	if (node == nullptr)
		return false;

	bool leftResult, rightResult;
	leftResult = Delete(node->Left, node, Data);
	// �����Ϳ� ���� �� ó���� �� ���� �ִ��� Ȯ�� �ʿ�
	if (Data == node->Data)
	{
		// �ڽ� �Ѵ� ���� ���
		if (node->Left != nullptr && node->Right != nullptr)
		{
			// ������ �� ���������� �����ϰ�, �ش� node ���� �� ����.
			Node* thisNode = node;
			Parent = node;
			node = node->Left;
			while (node->Right != nullptr)
			{
				Parent = node;
				node = node->Right;
			}
			thisNode->Data = node->Data;
			// ���⼭ return ���� �ʴ� ������ node �� ������ �� �Ʒ� ���� ���.
		}
		
		// Root �� Parent ���� ������ �ʿ䰡 ����.
		if (node == _Root)
		{
			if (node->Left != nullptr)
			{
				_Root = node->Left;
			}
			else if (node->Right != nullptr)
			{
				_Root = node->Right;
			}
			delete node;
			return true;
		}

		// ���� �ڽ��� �ִ� ���
		if (node->Left != nullptr)
		{
			if (Parent->Left == node)
			{
				Parent->Left = node->Left;
			}
			else
			{
				Parent->Right = node->Left;
			}
		}
		// ���� �ڽ��� �ִ� ���
		else if (node->Right != nullptr)
		{
			if (Parent->Left == node)
			{
				Parent->Left = node->Right;
			}
			else
			{
				Parent->Right = node->Right;
			}
		}
		// �ڽ��� ���� ���
		else
		{
			if (Parent->Left == node)
			{
				Parent->Left = nullptr;
			}
			else
			{
				Parent->Right = nullptr;
			}
		}
		delete node;
		return true;
	}
	rightResult = Delete(node->Right, node, Data);

	return leftResult || rightResult;
}

bool BinaryTree::Find(Node* node, int Data)
{
	if (node == nullptr)
		return false;

	bool leftResult, rightResult;

	leftResult = Find(node->Left, Data);
	if (node->Data == Data)
	{
		return true;
	}
	rightResult = Find(node->Right, Data);

	return leftResult || rightResult;
}

void BinaryTree::DeleteDestructor(Node* node)
{
	if (node == nullptr)
		return;

	// �Ҹ��ڴ� ���� ����
	DeleteDestructor(node->Left);
	DeleteDestructor(node->Right);
	delete node;
}

int BinaryTree::GetMaxDepth() const
{
	int maxLevel = 0;

	if (_Root == nullptr)
		return maxLevel;

	maxLevel = _Root->GetMaxDepth();

	return maxLevel;
}