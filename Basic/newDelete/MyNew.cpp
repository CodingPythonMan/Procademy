#include "MyNew.h"

#define new MyNew::operator new(__FILE__, __LINE__)
MyNew myNew;

MyNew::MyNew()
{
	printf("����!\n");
}

MyNew::~MyNew()
{
	printf("����!\n");
}
