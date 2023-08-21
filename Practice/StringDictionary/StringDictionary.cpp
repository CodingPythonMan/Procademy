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

}

// 5. strchr - ���ڿ� ������ Ư�� ���� �˻� (������ ����)
// 6. strstr - ���ڿ� ������ Ư�� ���ڿ� �˻� (������ ����)
// 7. strlwr - ���ڿ��� �ҹ��ڷ� ��ȯ

int main()
{
    char name[50] = "12312435";
    char name2[50];

    my_strcpy(name2, name);
    int cmp = my_strcmp(name, name2);
    
    printf("%d\n", cmp);
}