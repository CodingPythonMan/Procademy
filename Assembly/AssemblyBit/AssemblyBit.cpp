// AssemblyBit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void PrintBit(unsigned char a)
{
    printf("a : ");
    for (int i = 0; i < sizeof(a) * 8; i++)
    {
        printf("%d", (a & 1 << (7 - i)) >> (7 - i));
    }
}

void ChangeBit()
{
    unsigned short a = 0;
    while (true)
    {
        int pos = 0;
        printf("��Ʈ��ġ : ");
        scanf_s("%d", &pos);
        if (pos <= 0 || pos > 16)
        {
            printf("���� �� �� �ԷµǾ����ϴ�.");
            continue;
        }
        int input = 0;
        printf("OFF/ON [0,1] : ");
        scanf_s("%d", &input);

        if (input == 0)
        {
            a = a & (0 << pos);
        }
        else if(input == 1)
        {
            a = a | (1 << pos);
        }
        else
        {
            continue;
        }

        for (int i = 0; i < 16; i++)
        {
            printf("%d�� �ڸ� : %d\n", i + 1, (a & 1 <<(16-i)) >> (16-i));
        }
    }
}

void SharedBit()
{

}

int main()
{
    //unsigned char a = 101;

    //PrintBit(a);
    ChangeBit();
}