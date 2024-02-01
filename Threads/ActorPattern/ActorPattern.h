#pragma once

#include <list>
#include <string>
#include <process.h>
#include "RingBuffer.h"
#include "Lock.h"
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
unsigned int WINAPI Monitor(LPVOID lpParam);

bool MessageProc(short Type, wstring message);

//-----------------------------------------------
// ������ ��, ���ڿ� ����Ʈ
//-----------------------------------------------
extern list<wstring>	gList;

//-----------------------------------------------
// ������ �޽��� ť (������ �˳��ϰ� ũ�� 4~5�� ����Ʈ)
//-----------------------------------------------
extern RingBuffer		messageQ;

extern SRWLOCK			lock;
extern HANDLE			gEvent;
extern bool				MonitorTerminate;