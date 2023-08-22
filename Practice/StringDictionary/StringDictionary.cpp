#include <iostream>

// 1. strlen - ���ڿ� ���� ���
int my_strlen(char* str)
{
    int length = 0;
    
    while (*str != '\0')
    {
        str++;
        length++;
    }

    return length;
}

// 2. strcpy - ���ڿ� ����
char* my_strcpy(char *strDest, const char *strSrc)
{
    while (*strSrc != '\0')
    {
        *strDest = *strSrc;
        strDest++;
        strSrc++;
    }
    *strDest = '\0';

    return strDest;
}

// 3. strcmp - ���ڿ� ��
int my_strcmp(const char* string1, const char* string2)
{
    while (*string1 != '\0')
    {
        if (*string1 != *string2)
        {
            return 1;
        }
        string1++;
        string2++;
    }

    return 0;
}

// 4. strcat - ���ڿ� ���̱�
char* my_strcat(char* strDest, const char* strAdd)
{
    while (*strDest != '\0')
    {
        strDest++;
    }

    while (*strAdd != '\0')
    {
        *strDest = *strAdd;
        strDest++;
        strAdd++;
    }
    *strDest = '\0';

    return strDest;
}

// 5. strchr - ���ڿ� ������ Ư�� ���� �˻� (������ ����)
char* my_strchr(const char* string, int c)
{
    while (*string != '\0')
    {
		if (*string == c)
		{
			break;
		}
        string++;
        
        if (*string == '\0')
        {
            return NULL;
        }
    }

    return const_cast<char*>(string);
}

// 6. strstr - ���ڿ� ������ Ư�� ���ڿ� �˻� (������ ����)
// 7. strlwr - ���ڿ��� �ҹ��ڷ� ��ȯ

int main()
{
    int ch = '1';
    int result;

    char* dest;
    char string[80] = "abcdefghijklmn";

    dest = my_strchr(string, ch);
    result = (int)(dest - string + 1);

    printf("%d\n", result);
}