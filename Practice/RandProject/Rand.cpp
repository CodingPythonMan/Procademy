#include "Rand.h"

// �̱��� ȸ���� ���� ����� �ǰ� �־�� ��.
int iCount = 0;

st_ITEM g_Gatcha[] = {
	{"Į",				20, 0},
	{"����",				20, 0},
	{"�Ź�",				20, 0},
	{"����",				20, 0},
	{"�ʰ��·����",		5, 0},
	{"�ʰ��¹���",			5, 0},
	{"�ʰ��·�����Ź�1", 1, 50},
	{"�ʰ��·�����Ź�2", 1, 51},
	{"�ʰ��·�����Ź�3", 1, 10}

	// ������ 3���� �������� �Ϲ� Ȯ���δ� ������ ������
	// �ڿ� �Էµ� WinTime ȸ������ 100% �� ����.
};

void Gatcha()
{
	// �̱� ȸ�� ����. (�̴� ���������� ��� �Ǿ�� ��)
	iCount++;

	// 1. ��ü �����۵��� ���� �� ���� ����.
	// ��, WinTime �� ������ �������� Ȯ���� ��� ��ü�� �ƴϱ� ������ ����.
	int total = 0;
	int max = 0;
	for (const auto& item : g_Gatcha)
	{
		if (item.WinTime > 0)
		{
			if (max < item.WinTime)
			{
				max = item.WinTime;
			}
			continue;
		}
		
		total += item.Rate;
	}

	// 2. �� �̱� ȸ���� ���� ���� �������� �ִ��� Ȯ��
	// WinTime �� iCount �� ���� �������� ã�´�.
	// �ִٸ�..�� �������� �̰� �ߴ�.
	for (const auto& item : g_Gatcha)
	{
		if (item.WinTime == iCount)
		{
			printf("%d ���� ��� : %s\n", iCount, item.Name);
			if (iCount == max)
			{
				iCount = 0;
			}
			
			return;
		}
	}

	// 3. rand() �Լ��� Ȯ���� ����
	// ���⼭ Ȯ���� 1 / 100 �� �ƴϸ�, 1 / ���պ��� ��.

	// int iRand = Ȯ�����ϱ�;
	int random = (rand() % total) + 1;

	// 4. ��ü ������ ���̺��� ���鼭
	// ������ ���� Rand ���� �ش� ������ �������� ã�´�.
	for (const auto& item : g_Gatcha)
	{
		if (random <= item.Rate)
		{
			printf("%d ���� ��� : %s\n", iCount, item.Name);
			break;
		}

		random -= item.Rate;
	}

	// 5. �̱� ȸ���� �ʱ�ȭ �ؾ����� �Ǵ��Ͽ� �ʱ�ȭ.
}

int MakeBigNumRand()
{
	return 5;
}