#include <iostream>
#include <conio.h>
using namespace std;

struct st_ITEM
{
	char	Name[30];
	int	Rate;		// �Ϲ� ���� �̱�� ���� �뵵
	int	WinTime;	// �� �������� ���� �̱� ȸ��.
	// 0 �̸� �Ϲ� ������
	// 0 �� �ƴϸ� �� ȸ������ ����
};

void Gatcha();

// ū ���� Rand �����
int MakeIntRand();