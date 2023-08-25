#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Console.h"
#include "Main.h"
#include "StageRead.h"

void Player_Initial(void)
{
	//-------------------------------------------------------------------
	// �÷��̾� ���� ������ �ʱ�ȭ.
	// HP : 3 
	// 39,19 ��ġ���� ���� ��.
	//-------------------------------------------------------------------
	_Player.HP = 3;
	_Player.Visible = true;

	_Player.X = 39;
	_Player.Y = 19;
}

void Buffer_Flip(void)
{
	for (int count = 0; count < SCREEN_HEIGHT; count++)
	{
		cs_MoveCursor(0, count);
		printf(ScreenBuffer[count]);
	}
}

void Buffer_Clear(void)
{
	for (int count = 0; count < SCREEN_HEIGHT; count++)
	{
		memset(ScreenBuffer[count], ' ', SCREEN_WIDTH);
		ScreenBuffer[count][SCREEN_WIDTH - 1] = (char)NULL;
	}
}

void Sprite_Draw(int X, int Y, char Sprite)
{
	if (X < 0 || Y < 0 || X >= SCREEN_WIDTH - 1 || Y >= SCREEN_HEIGHT)
		return;

	ScreenBuffer[Y][X] = Sprite;
}

bool KeyProcess(void)
{
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_LEFT))
	{
		_Player.X -= 1;
	}
	// ������ ����Ű.
	if (GetAsyncKeyState(VK_RIGHT))
	{
		_Player.X += 1;
	}
	// ���� ����Ű.
	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		_Player.Y--;
	}
	// �Ʒ��� ����Ű.
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		_Player.Y++;
	}

	//-------------------------------------------------------------
	// �÷��̾� �̵� �ݰ� ����.
	// ���� ȭ�鿡�� �÷��̾ �̵� ������ ������ �����Ѵ�.
	//-------------------------------------------------------------

	_Player.X = max(_Player.X, 0);
	_Player.X = min(_Player.X, 79);
	_Player.Y = max(_Player.Y, 0);
	_Player.Y = min(_Player.Y, 23);


	// ��Ʈ�� Ű. (�̻��� Ű)
	if (GetAsyncKeyState(VK_CONTROL))
	{
		_Missiles[_MissileCount].X = _Player.X;
		_Missiles[_MissileCount].Y = _Player.Y - 1;
		_Missiles[_MissileCount].Visible = 1;

		_MissileCount++;
	}

	// ESC Ű. (����)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}


	return true;
}

void Draw_Player(void)
{
	if (_Player.Visible)
		Sprite_Draw(_Player.X, _Player.Y, '#');

}

void Missile_Move(void)
{
	for (int i = 0; i < _MissileCount; i++)
	{
		if (_Missiles[i].Visible)
		{
			_Missiles[i].Y += -1;
		}

		// �� �ε���
		if (_Missiles[i].Y < 0 || _Missiles[i].Y >= SCREEN_HEIGHT)
		{
			_MissileCount--;
			for (int j = i; j < _MissileCount; j++)
			{
				_Missiles[j] = _Missiles[j + 1];
			}
			i--;
		}
	}
}

void Draw_Missile(void)
{
	for (int i = 0; i < _MissileCount; i++)
	{
		if (_Missiles[i].Visible)
		{
			Sprite_Draw(_Missiles[i].X, _Missiles[i].Y, '|');
		}
	}
}

void Draw_Monster(void)
{
	for (int i = 0; i < _MonsterCount; i++)
	{
		if (_Monsters[i].Visible)
		{
			Sprite_Draw(_Monsters[i].X, _Monsters[i].Y, 'M');
		}
	}
}

void Monster_Set(int stage)
{
	char* buffer = sr_Stage_Read(_Stages[stage]);

	int defaultX = 31;
	int defaultY = 5;
	for (int i = 0; i < (int)strlen(buffer); i++)
	{
		if (buffer[i] == '\t')
		{
			defaultX += 4;
		}
		if (buffer[i] == '\n')
		{
			defaultX = 31;
			defaultY += 1;
		}
		if (buffer[i] == 'M')
		{
			_Monsters[_MonsterCount].X = defaultX;
			_Monsters[_MonsterCount].Y = defaultY;
			_Monsters[_MonsterCount].Visible = 1;
			_MonsterCount++;
		}
	}
}

void Monster_Move()
{
	for (int i = 0; i < _MonsterCount; i++)
	{
		if (_Monsters[i].Visible)
		{
			if (_Monsters[i].left > 0)
			{
				_Monsters[i].left--;
				_Monsters[i].X -= 1;
			}
			else if (_Monsters[i].left == 0 && _Monsters[i].right > 0)
			{
				_Monsters[i].right--;
				_Monsters[i].X += 1;
			}
			else if (_Monsters[i].left == 0 && _Monsters[i].right == 0)
			{
				_Monsters[i].left = 4;
				_Monsters[i].right = 4;
			}
		}
	}
}

bool Check_Survive()
{
	for (int i = 0; i < _MissileCount; i++)
	{
		for (int j = 0; j < _MonsterCount; j++)
		{
			if (_Missiles[i].X == _Monsters[j].X && _Missiles[i].Y == _Monsters[j].Y)
			{
				_MissileCount--;
				for (int k = i; k < _MissileCount; k++)
				{
					_Missiles[k] = _Missiles[k + 1];
				}
				i--;

				_MonsterCount--;
				_Monsters[j].Visible = 0;
			}
		}
	}

	if (_MonsterCount == 0)
		return true;

	return false;
}

int main(void)
{
	cs_Initial();
	//Map_Set();

	for (int stage = 0; stage < _StageCount; stage++)
	{
		Monster_Set(stage);
		Player_Initial();

		//--------------------------------------------------------------------
		// ������ ���� ����
		// �� ������  1�� ���� 1������ �̴�.
		//--------------------------------------------------------------------
		while (1)
		{
			// Ű���� �Է�
			bool keyResult = KeyProcess();

			// ������
			if (keyResult == false)
				return 0;

			Missile_Move();
			Monster_Move();

			bool result = Check_Survive();
			if (result == true)
				break;

			// ������
			Buffer_Clear();
			Draw_Player();
			Draw_Missile();
			Draw_Monster();
			Buffer_Flip();

			// ������ ����
			Sleep(200);
		}
	}
}