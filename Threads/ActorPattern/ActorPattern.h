#include <process.h>
#include <list>
#include <string>
#include <windows.h>
#include "RingBuffer.h"
using namespace std;

#define WORKER_THREAD_NUMBER 3

#define dfJOB_ADD	0
#define dfJOB_DEL	1
#define dfJOB_SORT	2
#define dfJOB_FIND	3
#define dfJOB_PRINT	4	//<< ��� or ���� / �б⸸ �ϴ� ���� �ൿ
#define dfJOB_QUIT	5	

//#pragma pack(push, 1)
struct st_MSG_HEAD
{
	short shType;
	short shPayloadLen;
};
//#pragma pack(pop)

unsigned int WINAPI Work(LPVOID lpParam);

//-----------------------------------------------
// ������ ��, ���ڿ� ����Ʈ
//-----------------------------------------------
extern list<wstring>	gList;

//-----------------------------------------------
// ������ �޽��� ť (������ �˳��ϰ� ũ�� 4~5�� ����Ʈ)
//-----------------------------------------------
extern RingBuffer		messageQ;

extern SRWLOCK lock;