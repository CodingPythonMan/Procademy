#include "Dummy.h"
#include <iostream>
using namespace std;

int main()
{
	cout << "[Dummy System] �� ���� ���� ȯ���մϴ�.\n";
	int menu = 0;

	while (1)
	{
		cout << "[�޴�]\n";
		cout << "\t1. TCP Fighter ���� �����(40) \n";
		cin >> menu;
		
		switch (menu)
		{
		case 1:
			cout << "1�� ���õǾ����ϴ�. \n";
			
		default:
			return;
		}
	}
}