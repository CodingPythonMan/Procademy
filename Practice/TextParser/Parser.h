#pragma once
#include <windows.h>

class Parser
{
private:
	Parser();
	~Parser();

	bool GetNextWord(char** chrBuffer, int* length);
	bool SkipToNextWord(char** chrBufferPtr);

	char* buffer;

public:
	static Parser* GetInstance();

	void LoadFile(const char* fileName);

	// Get �ϴ� �Լ���
	bool GetValueInt(const char* valueName, int* column);
	bool GetValueStr(const char* valueName, char** column);
	bool GetValueWStr(const char* valueName, WCHAR** column);

	int Version;
	int ServerID;
	char* Path;
	char* FileName;
	WCHAR* IP;
};