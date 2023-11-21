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

	fread_s(buffer, length + 1, 1, length, file);
	buffer[length] = '\0';

	fclose(file);
}

bool Parser::GetValueInt(const char* valueName, int* column)
{
	int length = 0;
	char word[256];
	char* chrBuffer = buffer;
	
	while (1)
	{
		if (GetNextWord(&chrBuffer, &length))
		{
			// Word ���ۿ� ã�� �ܾ� ����
			memset(word, 0, 256);
			memcpy(word, chrBuffer, length);
			word[255] = '\0';

			chrBuffer += length;
			// ���ڷ� �Է¹��� �ܾ�� ������ �˻��Ѵ�.
			if (0 == strcmp(valueName, word))
			{
				// �ٷ� �� = �� ã�´�.
				if (GetNextWord(&chrBuffer, &length))
				{
					memset(word, 0, 256);
					memcpy(word, chrBuffer, length);

					chrBuffer += length;
					if (0 == strcmp(word, "="))
					{
						// �ٷ� �� ���� ã�´�.
						if (GetNextWord(&chrBuffer, &length))
						{
							memset(word, 0, 256);
							memcpy(word, chrBuffer, length);
							*column = atoi(word);
							return true;
						}
						return false;
					}
				}
				return false;
			}
		}
	}

	return false;
}

bool Parser::GetNextWord(char** chrBufferPtr, int* lengthPtr)
{
	// length �� ó���� �ʱ�ȭ
	*lengthPtr = 0;

	// ���� ��ȿ ���� ù��°���� �̵�
	if (SkipToNextWord(chrBufferPtr))
	{
		return false;
	}

	char* chrBuffer = *chrBufferPtr;
	while (1)
	{
		if (*chrBuffer == ',' || *chrBuffer == '"' || *chrBuffer == 0x20 || *chrBuffer == 0x08 
			|| *chrBuffer == 0x09 || *chrBuffer == 0x0a || *chrBuffer == 0x0d)
		{
			break;
		}

		chrBuffer++;
		(*lengthPtr)++;
	}

	if (*chrBuffer == '\0')
		return false;
	
	return true;
}

bool Parser::SkipToNextWord(char** chrBufferPtr)
{
	while (1)
	{
		char* chrBuffer = *chrBufferPtr;

		// �ּ�ó�� ��ŵ 
		if (*chrBuffer == '/')
		{
			if (*(chrBuffer + 1) == '\0')
			{
				break;
			}

			if (*(chrBuffer + 1) == '/')
			{
				// New Line ó������ ��ŵ
				while (1)
				{
					if (*chrBuffer == 0x0d && *(chrBuffer + 1) == 0x0a)
						break;

					if (*chrBuffer == '\0')
						break;

					(*chrBufferPtr)++;
				}
			}
		}

		// �����̽�, ��, �����ڵ�
		// 0x20 �����̽�
		// 0x08 �齺���̽�
		// 0x09 ��
		// 0x0a ���� �ǵ�
		// 0x0d ĳ���� ����
		if (*chrBuffer == 0x20 || *chrBuffer == 0x08 || *chrBuffer == 0x09 || 
			*chrBuffer == 0x0a || *chrBuffer == 0x0d || *chrBuffer == '{')
		{
			(*chrBufferPtr)++;
		}
		else
		{
			break;
		}
	}
	
	if(**chrBufferPtr == '\0')
		return true;

	return false;
}