#include "Player.h"
#include <iostream>

Player::Player()
{
	_Fly = 0;
	_X = 50;
	_Y = 50;
	memset(_Name, 0, 30);

	printf("Player ������ ȣ��! \n");
}

Player::~Player()
{
	printf("Player �Ҹ��� ȣ��! \n");
}

void Player::MakeName(const char* Name)
{
	memcpy(_Name, Name, 30);
}

void Player::Move(int X, int Y)
{
	_X = X;
	_Y = Y;

	printf("_X : %d, _Y : %d\n", _X, _Y);
}

void Player::NotifyPos()
{
	//printf("_X : %d, _Y : %d\n", _X, _Y);
}