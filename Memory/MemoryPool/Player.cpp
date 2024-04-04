#include "Player.h"
#include <iostream>

Player::Player()
{
	_Fly = 0;
	_X = 50;
	_Y = 50;
	memset(_Name, 0, 30);

#ifdef _DEBUG
	printf("Player ������ ȣ��! \n");
#endif
}

Player::~Player()
{
#ifdef _DEBUG
	printf("Player �Ҹ��� ȣ��! \n");
#endif
}

void Player::OnlyOneAdd()
{
	_Fly++;
}

void Player::MakeName(const char* Name)
{
	memcpy(_Name, Name, 30);
}

void Player::Move(int X, int Y)
{
	_X = X;
	_Y = Y;

#ifdef _DEBUG
	printf("_X : %d, _Y : %d\n", _X, _Y);
#endif
}

void Player::NotifyPos()
{
#ifdef _DEBUG
	printf("_X : %d, _Y : %d\n", _X, _Y);
#endif
}