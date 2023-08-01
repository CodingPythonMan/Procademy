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
    unsigned short value = 0;
    unsigned short temp;

    int bitSize = sizeof(value) * 8;

    while (true)
    {
        int pos = 0;
        int input = 0;
        printf("��Ʈ��ġ : ");
        scanf_s("%d", &pos);
        if (pos <= 0 || pos > bitSize)
        {
            printf("���� �� �� �ԷµǾ����ϴ�.");
            continue;
        }
        
        printf("OFF/ON [0,1] : ");
        scanf_s("%d", &input);

        if (input != 0)
        {
            value = value | (1 << (pos-1));
        }
        else
        {
            temp = ~(1 << (pos-1));
            value = value & temp;
        }

        for (int i = 0; i < bitSize; i++)
        {
            temp = value & (1 << (bitSize - i - 1));
            
            printf("%d �� Bit : ", bitSize - i);
            
            if (temp != 0)
                printf("ON");
            else
                printf("OFF");

            printf("\n");
        }
        printf("\n");
    }
}

void SharedBit()
{
    unsigned int value = 0;
    unsigned int temp;

    int byteSize = sizeof(value);
    int bitSize = byteSize * 8;

    for (;;)
    {
        unsigned int mask = 0xFFFFFF00;
		int pos = 0;
        printf("����Ʈ ��ġ : ");
		scanf_s("%d", &pos);

        if (pos < 1 || pos > byteSize)
        {
            printf("�߸��� ���� �Է� �Ǿ����ϴ�.\n");
            continue;
        }

        printf("0~255 : ");
        scanf_s("%d", &temp);

        if (temp < 1 || temp > 255)
        {
			printf("�߸��� ���� �Է� �Ǿ����ϴ�.\n");
			continue;
        }

        mask = mask << (pos - 1);
        value = value & mask;
        temp = temp << ((pos - 1) * 8);
        value = value | temp;

        // Bit ���� ���
        printf("Bit : ");
        for (int i = 0; i < bitSize; i++)
        {
            temp = value & (1 << (bitSize - i - 1));

            if (temp != 0)
                printf("%d", 1);
            else
                printf("%d", 0);
        }

        printf("\nByte : 0x");
        // Byte ���� ���
        for (int i = 0; i < byteSize; i++)
        {
            mask = 0x000000FF;
            temp = value & (mask << (byteSize - i - 1));
            unsigned char a = temp >> (byteSize - i - 1);
            printf("%x", a);
        }
        printf("\n");
    }
}

int main()
{
    //unsigned char a = 101;

    //PrintBit(a);
    //ChangeBit();
    SharedBit();
}