#include "MyNew.h"

struct Player {
	Player() : _x(0), _y(0)
	{
		printf("������ ȣ��\n");
	}

	~Player()
	{
		printf("�Ҹ��� ȣ��\n");
	}

	int _x;
	int _y;
};

int main()
{
	Player* p = new Player;
	
	delete p;
}