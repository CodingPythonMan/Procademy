#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser::Parser() : Version{ 0 }, ServerID{ 0 }, buffer{ nullptr }
{
}

Parser::~Parser()
{
	free(buffer);
}

Parser* Parser::GetInstance()
{
	static Parser parser;
	return &parser;
}

void Parser::LoadFile(const char* fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "r+");

	if (file == nullptr)
		return;

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(length + 1);
	if (buffer == nullptr)
		return;

	fread_s(buffer, length + 1, 0, length, file);
	buffer[length] = '\0';

	fclose(file);
}

bool Parser::GetValueInt(const char* valueName, int* column)
{
	int length = 0;
	char word[256];
	char* chrBuffer = buffer;
	
	// Load ��� { �� ã�� �����Ѵ�.
	while (1)
	{
		if (*chrBuffer == '}')
			break;

		if (*chrBuffer == '\0')
			return;

		chrBuffer++;
	}

	while (GetNextWord(&chrBuffer, &length))
	{
		// Word ���ۿ� ã�� �ܾ� ����
		memset(word, 0, 256);
		memcpy(word, chrBuffer, length);
		word[255] = '\0';

		// ���ڷ� �Է¹��� �ܾ�� ������ �˻��Ѵ�.
		if (0 == strcmp(valueName, word))
		{
			// �ٷ� �� = �� ã�´�.

		}
	}

	return false;
}

bool Parser::GetNextWord(char** chrBufferPtr, int* lengthPtr)
{
	char* chrBuffer = *chrBufferPtr;

	while (1)
	{
		if (SkipNoneCommand(chrBuffer))
		{
			return false;
		}
		
		if (*chrBuffer == ',' || *chrBuffer == '.' || *chrBuffer == '"' || *chrBuffer == 0x08 || *chrBuffer == '}')
		{
			break;
		}

		chrBuffer++;
	}

	if (*chrBuffer == '}')
		return false;
	
	return true;
}

bool Parser::SkipNoneCommand(char* chrBuffer)
{
	while (1)
	{
		// �ּ�ó�� ��ŵ


		// �����̽�, ��, �����ڵ�
		// 0x20 �����̽�
		// 0x08 �齺���̽�
		// 0x09 ��
		// 0x0a ���� �ǵ�
		// 0x0d ĳ���� ����
		if (*chrBuffer == 0x20 || *chrBuffer == 0x09 || *chrBuffer == 0x0a || *chrBuffer == 0x0d)
		{
			chrBuffer++;
		}
		else
		{
			break;
		}
	}
	
	if(*chrBuffer == '\0' || *chrBuffer == '}')
		return true;

	return false;
}