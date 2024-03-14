#pragma once
#include <Windows.h>
#include <profileapi.h>
#include <cstdio>
#include <locale.h>

#define PROFILE_MIN_COUNT 2
#define PROFILE_MAX_COUNT 2

#define MAX_PROFILE 20

class Profiler {
public:
	static Profiler* GetInstance(void)
	{
		static Profiler profiler;
		return &profiler;
	}

private:
	Profiler();
	~Profiler();

public:
	/////////////////////////////////////////////////////////////////////////////
	// �ϳ��� �Լ� Profiling ����
	/////////////////////////////////////////////////////////////////////////////
	void ProfileBegin(const WCHAR* szName);

	/////////////////////////////////////////////////////////////////////////////
	// �ϳ��� �Լ� Profiling ��
	/////////////////////////////////////////////////////////////////////////////
	void ProfileEnd(const WCHAR* szName);

	/////////////////////////////////////////////////////////////////////////////
	// Profiling �� ����Ÿ�� Text ���Ϸ� ����Ѵ�.
	/////////////////////////////////////////////////////////////////////////////
	void ProfileDataOutText(const WCHAR* szFileName);

	/////////////////////////////////////////////////////////////////////////////
	// �������ϸ� �� �����͸� ��� �ʱ�ȭ �Ѵ�.
	/////////////////////////////////////////////////////////////////////////////
	void ProfileReset(void);

	/////////////////////////////////////////////////////////////////////////////
	// ���� �Լ�
	/////////////////////////////////////////////////////////////////////////////
	void ProfileCallFunc(int index, const WCHAR* szName);

private:
	int TLSIndex;

	LARGE_INTEGER Freq;
};

struct ProfileInfo
{
	__int64			_call;						// ���� ȣ�� Ƚ��.
	WCHAR			_name[64];					// �������� ���� �̸�.

	LARGE_INTEGER	_startTime;					// �������� ���� ���� �ð�.

	__int64			_totalTime;					// ��ü ���ð� ī���� Time.	(��½� ȣ��ȸ���� ������ ��� ����)
	__int64			_min[PROFILE_MIN_COUNT];	// �ּ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ּ� [1] ���� �ּ� [2])
	__int64			_max[PROFILE_MAX_COUNT];	// �ִ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ִ� [1] ���� �ִ� [2])

	ProfileInfo()
	{
		_call = 0;
		_totalTime = 0;
	}
};