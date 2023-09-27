#include <iostream>

struct Way {
	int cashLine;
};

struct Cache {
    Way way[8];
};

Cache cache[64];

void CashSimulate(int* pointer)
{
    unsigned int* address = reinterpret_cast<unsigned int*>(pointer);
    unsigned int value = reinterpret_cast<unsigned int>(address);

    printf("%p\n", value);

    //printf("Cache Hit!");

    //printf("Cache Miss");
}

int main()
{
    int* pointer;

    int a = 0;
    pointer = &a;
    CashSimulate(pointer);
    int b = 0;

    a++;
    pointer = &a;
    CashSimulate(pointer);
    b++;

    a = b + 1;
    pointer = &a;
    CashSimulate(pointer);
}