// AssemblyBit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

void PrintBit(unsigned char a)
{
    printf("a : ");
    int bit[8];
    for (int i = 0; i < sizeof(a) * 8; i++)
    {
        bit[i] = a % 2;
        a /= 2;
    }

    for (int i = sizeof(bit) / sizeof(int) -1; i >= 0; i--)
    {
        printf("%d", bit[i]);
    }
}

void ChangeBit()
{
    int bit[16];

    while (true)
    {
        int pos = 0;
        printf("��Ʈ �ڸ� : ");
        scanf_s("%d", &pos);
        if (pos <= 0 || pos > 16)
        {
            printf("���� �� �� �ԷµǾ����ϴ�.");
            continue;
        }
        int input = 0;
        printf("�Է� : ");
        scanf_s("%d", &input);
        bit[pos] = input;

        for (int i = 0; i < 16; i++)
        {
            printf("%d�� �ڸ� : %d", i + 1, bit[i]);
        }
    }
}

int main()
{
    //unsigned char a = 101;
    unsigned char a = 101;

    //PrintBit(a);
    ChangeBit();
}