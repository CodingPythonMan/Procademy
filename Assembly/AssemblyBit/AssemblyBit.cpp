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
	unsigned int data = 0;
	unsigned int mask = 0;
	unsigned char byByteData = 0;

	int inputPos;
	int inputData;
	unsigned char byInputData;
	int count;

	while (1)
	{
		printf("��ġ (1~4) : ");
		scanf_s("%d", &inputPos);
		printf("�� [0~255] : ");
		scanf_s("%d", &inputData);

		if (inputPos <= 0 || inputPos >= 5)
		{
			printf("��ġ ������ �ʰ��Ͽ����ϴ�. \n\n");
			continue;
		}

		if (inputData < 0 || inputData > 255)
		{
			printf("������ ������ �ʰ��Ͽ����ϴ�. \n\n");
			continue;
		}
		byInputData = inputData;
		mask = 0xff << (inputPos - 1) * 8;
		data = data & ~mask;
		data = data | (byInputData << (inputPos - 1) * 8);

		for (count = 0; count < 4; count++)
		{
			mask = 0xff << count * 8;
			byByteData = (data & mask) >> count * 8;
			printf("%d ��° ����Ʈ �� : %d\n", count + 1, byByteData);
		}
		printf("\n");
		printf("��ü 4����Ʈ �� : 0x%08x \n\n", data);
	}
}

int main()
{
    //unsigned char a = 101;

    //PrintBit(a);
    //ChangeBit();
    SharedBit();
}