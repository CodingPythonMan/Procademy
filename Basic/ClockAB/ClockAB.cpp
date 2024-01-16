#include <iostream>
#include <windows.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define WAIT 20

int CallA = 0;
int CallB = 0;

void A()
{
	CallA++;
	//cout << "A()" << "\n";
}

void B()
{
	CallB++;
	//cout << "B()" << "\n";
}

int main()
{
	timeBeginPeriod(1);

	unsigned int curTime = timeGetTime();
	unsigned int frameTime = curTime;
	int Frame = 0;
	while (1)
	{
		A();

		if (Frame < 1000 / WAIT)
		{
			B();
			Frame++;
		}
		
		curTime = timeGetTime();
		if(curTime - frameTime >= 1000)
		{
			printf("curTime : %d, frameTime : %d\n", curTime, frameTime);
			printf("Frame : %d\n", Frame);
			printf("CallA : %d\n", CallA);
			printf("CallB : %d\n", CallB);

			// 1�ʵ��� ��ǥġ���� ���� ���Ҵٸ�, �� ���� Frame ����
			// �׷��� 1�� ������ �ƴ϶� �����ʵ��� �� ������ �� ����.
			Frame -= (1000 / WAIT);
			frameTime += 1000;
		}
	}
}