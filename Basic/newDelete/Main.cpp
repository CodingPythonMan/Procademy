#include "MyNew.h"
#include <stdio.h>

struct Player {
	Player() : _x(0), _y(0)
	{

	}

	~Player()
	{
		printf("�Ҹ��� ȣ��");
	}

	int _x;
	int _y;
};

int main()
{
	Player* p = new Player;
	
	delete p;
}