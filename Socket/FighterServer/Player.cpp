#include "Player.h"
#include <cstdlib>

Player::Player()
{
	Direct = Direction::RR;
	// ��ġ�� �����ϰ� ����
	X = RANGE_MOVE_LEFT + (rand() % (RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT)) + 1;
	Y = RANGE_MOVE_TOP + () + 1;
	HP = DEFAULT_HP;
}

Player::~Player()
{
}

bool Player::IsDead()
{
	
}

bool Player::MovePlayer()
{

}