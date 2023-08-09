#include <iostream>
#include <conio.h>

struct st_ITEM
{
    char Name[30];
    int Rate;
};

st_ITEM g_Gatcha[] = {
	{"Į",		20},
	{"����",		20},
	{"�Ź�",		20},
	{"����",		20},
	{"�ʰ��·����",	5},
	{"�ʰ��¹���",		5},
	{"�ʰ��·�����Ź�", 1}
};

void Gatcha()
{
	// 1. ��ü �����۵��� ���� �� ���� ����
	int total = 0;
	int index = 0;
	for (const auto& item : g_Gatcha)
	{
		total += item.Rate;
	}

	// 2. rand() �Լ��� Ȯ���� ����
	// ���⼭ Ȯ���� 1/100 �� �ƴϸ�, 1/���պ�����

	// int iRand = Ȯ�� ���ϱ�
	int random = (rand() % total) + 1;

	// 3. ��ü ������ ���̺��� ���鼭
	// ������ ���� Rand ���� �ش� ������ �������� ã�´�.
	while (random > g_Gatcha[index].Rate)
	{
		random -= g_Gatcha[index].Rate;
		index++;
	}
	printf("��í ��� : %s\n", g_Gatcha[index].Name);
}


int main()
{
	while (1)
	{
		_getch();
		Gatcha();
	}
}