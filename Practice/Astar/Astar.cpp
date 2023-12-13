#include "Astar.h"
#include "Tile.h"
#include <algorithm>

int dx[DIRECTION] = { 1,1,0,-1,-1,-1,0,1 };
int dy[DIRECTION] = { 0,-1,-1,-1,0,1,1,1 };

Astar::Astar()
{
	_Start = new Node;
	_End = new Node;
}

Astar::~Astar()
{
	delete _Start;
	delete _End;
}

void Astar::RoutingStart(HWND hWnd)
{
	// 1. �켱 �������� OpenList �� �ִ´�.
	_OpenList.push_back(_Start);

	// 2. �ݺ����� ����.
	while (1)
	{
		if (_OpenList.size() <= 0)
			return;

		sort(_OpenList.begin(), _OpenList.end(), [](const Node* o1, const Node* o2) {
			return o1->F > o1->F;
		});
		// 3. F�� ���� ���� ���̸� �����Ѵ�.
		Node* node = _OpenList[(int)_OpenList.size() - 1];
		_OpenList.pop_back();
		
		// 4. ������ ����.
		if (node->X == _End->X && node->Y == _End->Y)
		{
			
		}

		for (int i = 0; i < DIRECTION; i++)
		{

		}
	}
}