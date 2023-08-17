#include <iostream>
#include <Windows.h>
#include <conio.h>

int g_Timing[] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };

// Ÿ�̹� ������
// �ڷ� �� ���� ū ���� �ִ°� ��Ģ

// # ȭ�� ����� system("cls")

// # �ð����ϱ� - clock() �Լ�
// ���μ����� ���۵� ���� �ð�, 1�ʴ� CLOCKS_PER_SEC �� (1000ms)
// ���ϰ��� clock_t (long) Ÿ��

// # Ű���� ���ȴ��� Ȯ�� - _kbhit() �Լ�
// Ű���尡 ���ȴ��� Ȯ�� �� �������� true ����
// ��, Ű�� ���� �� �ܼ�Ű���� ���� ������ ������ ���� ���������� ��� true ����

// _getch() �Լ��� ����Ͽ� �ܼ�Ű �Է��� ���� �� ����.
// ���� ���� char keyChar = _getch();
// �츮�� _getch() �� ���ϰ��� ������� ������ �ܼ�Ű�� ���� ���� ��.

// # abs(X) ���밪 �Լ�

// - ������ ��
// 1. ȭ�� ��ܿ� �ð��� ǥ�� ��. �� : �и������� (00:000 ���� �ڸ��� ����)
// 2. �Ʒ����� �� Ű Ÿ�̹��� ������ �ش� Ÿ�ֿ̹� �������� ����� ǥ��
// 3. �ƹ��� Ű�� ������ �ʰ� ���� �ð��� 1�� �̻� ������ �ڵ����� fail ó��
// 4. ����ڰ� Ű�� ������ �ش� �ð��� üũ�Ͽ� ���� ������ ���� ���� Ÿ�̹��� ����� ȭ�鿡 ǥ�õ�.

// CLOCKS_PER_SEC / 4 �� ��ġ�� 1�ܰ�� �ִ� 4�ܰ� ������ ������ Great, Good, NoGood, Bad �ܰ� ����
// ������� ����, + - ��� �ش��.

int main()
{
	const int timingSize = sizeof(g_Timing) / sizeof(int);
	double timing[timingSize];
	const char* grade[timingSize];
	for (int i = 0; i < timingSize; i++)
	{
		timing[i] = g_Timing[i];
		grade[i] = "";
	}

	int index = 0;

    while (1)
    {
		// �Էº�
		clock_t time = clock();
		double sec = static_cast<double>(time) / CLOCKS_PER_SEC;
		double difference;
		
		if (_kbhit())
		{
			_getch();
			// ������ ����
			difference = abs(sec - timing[index]);
			
			if (difference > 1)
			{
				grade[index] = "Bad";
			}
			else if (difference > 0.75)
			{
				grade[index] = "NoGood";
			}
			else if (difference > 0.5)
			{
				grade[index] = "Good";
			}
			else
			{
				grade[index] = "Great";
			}

			index++;
		}

		// ������
		difference = sec - timing[index];
		if (difference > 1)
		{
			grade[index] = "Fail";
			index++;
		}

		if (timingSize <= index)
		{
			break;
		}
		
		// ������
		printf("%06.3f Sec\n\n", sec);
		for (int i=0; i<timingSize; i++)
		{
			printf("%d Sec : %s\n", g_Timing[i], grade[i]);
		}
		system("cls");
    }
}
