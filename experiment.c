#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
char Word[32];
int ch;//�洢ʶ��ĵ���
int number[1000][100];//������,number[][0]�洢��ǰ�еĶ�����λ��
char mark[100][5];//��ʶ��
int line = 0, row = 0;
const char *key[10] = { "void","char","int","float","double","if","else","for","do","while"};
int Isletter(char c)//�ж��Ƿ�����ĸ
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	else return 0;
}
int Isnumber(char c)//�ж��Ƿ�Ϊ����
{
	if (c >= '0' && c <= '9')
		return 1;
	else return 0;
}
int Iskeyword(char* Word)
{
	int i, m;
	for (i = 0; i < 10; i++)
	{
		if (m = strcmp(Word, key[i]) == 0)//���ִ�Сд�Ƚ�
			return i + 1;//�ڹؼ��ֱ��д���
	}
		if (line != 0)//�ж��Ƿ��ڱ�ʶ������
		{
			int s = 0;
			while (s < line)
			{
				if (strcmp(Word, mark[s++]) == 0)//����
				{
					printf("%s\t(6,%d)\n\n", Word, s);
					return 1;//Ϊ��ʶ��
				}
			}

		}
		strcpy(mark[line], Word);//����������뵽��ʶ������
		printf("%s\t(6,%d)\n\n", Word, line + 1);
		line++;
		return 1;

}
int IsDecimal(char *Word) 
{
    if (Word == NULL || *Word == '\0') 
        return 0; // ���ַ���������Ч������
    int i = 0;
    int hasDecimalPoint = 0; // ���С������ֵĴ���
    // ����һ���ַ��Ƿ�Ϊ������
    if (Word[i] == '-') 
        i++; // ��������
    // ���ʣ����ַ�
    for (; Word[i] != '\0'; i++) 
	{
        if (Word[i] == '.') 
		{
            // ����Ѿ���С���㣬�ٴγ���С�����򲻺Ϸ�
            if (hasDecimalPoint)
			return 0;
            hasDecimalPoint = 1; // ��һ������С����
        } 
		else if (!Isnumber(Word[i]))
           {
			printf("��������");// ����������֣��Ҳ���С���㣬���ز�������
            return 0;
			}
    }
    // ����ַ���ȫ�ǺϷ���С��������1,��������2
	if(hasDecimalPoint)
    return 1;
	else return 2;
}
void scan(FILE* fp)//ɨ����
{
	char Word[32] = { '\0' };
	char ch;
	int i, c,d;
	ch = fgetc(fp);//��ȡһ���ַ�
	if (Isletter(ch))
	{
		Word[0] = ch;
		ch = fgetc(fp);
		i = 1;
		while (Isnumber(ch) || Isletter(ch))
		{
			Word[i] = ch;
			i++;
			ch = fgetc(fp);
		}
		Word[i] = '\0';
		fseek(fp, -1, 1);
		c = Iskeyword(Word);
		if (c != 1)
			printf("%s\t(1,%d)\n\n",Word, c);
	}
	else if(Isnumber(ch))
	{
		Word[0] = ch;
		ch = fgetc(fp);
		i = 1;
		if (Isletter(ch))
		{
			printf("��ʶ����ʽ����");
		}
		while (Isnumber(ch))
		{
			Word[i] = ch;
			i++;
			ch = fgetc(fp);
		}
		d = IsDecimal(Word);
		if(d==1)
		{
			double num1 = atof(Word); 
			printf("%s\t(5,%d)\n\n",Word,d);
		}
		else if(d==2)
	    {
			int num2 = atoi(Word);
			printf("%s\t(5,%d)\n\n",Word,d);
		}
		fseek(fp, -1, 1);
	}
	else
	{
		Word[0] = ch;
		i = 1;
		switch (ch)
		{
		case',':
			printf("%s\t(2,0)\n\n", Word);
			break;
		case ';':
			printf("%s\t(2,1)\n\n", Word);
			break;
		case'.':
			printf("%s\t(2,2)\n\n", Word);
			break;
		case':':
			ch = fgetc(fp);
			Word[1] = ch;
			if (ch == '=')
			{
				printf("%s\t(2,3)\n\n", Word);
			}
			else
			{
				fseek(fp, -1, 1);
				printf("%s\t(2,6)\n\n", Word);
			}
			break;
		case')':
			printf("%s\t(2,5)\n\n", Word);
			break;
		case'(':
			printf("%s\t(2,4)\n\n", Word);
			break;
		case'+':
			printf("%s\t(3,1)\n\n", Word);
			break;
		case'-':
		    ch = fgetc(fp);
			if(Isnumber(ch))
			{
				while (Isnumber(ch))
		         {
			       Word[i] = ch;
			       i++;
			       ch = fgetc(fp);
		         }
		       d = IsDecimal(Word);
		       if(d==1)
		         {
			       double num1 = atof(Word); 
			       printf("%s\t(5,%d)\n\n",Word,d);
		         }
		       else if(d==2)
	             {
			      int num2 = atoi(Word);
			      printf("%s\t(5,%d)\n\n",Word,d);
		         }
			}
			else printf("%s\t(3,2)\n\n", Word);
			break;
		case'*':
			printf("%s\t(3,3)\n\n", Word);
			break;
		case'/':
			printf("%s\t(3,4)\n\n", Word);
			break;
		case'<':
			ch = fgetc(fp);
			Word[1] = ch;
			if (ch == '=')
			{
				printf("%s\t(4,1)\n\n", Word);
			}
			else if (ch == '>')
			{
				printf("%s\t(4,5)\n\n", Word);
			}
			else
			{
				fseek(fp, -1, 1);
				printf("%c\t(4,0)\n\n", Word[0]);
			}
			break;
		case'=':
			printf("%s\t(4,2)\n\n", Word);
			break;
		case'>':
			ch = fgetc(fp);
			Word[1] = ch;
			if (ch == '=')
			{
				printf("%s\t(4,4)\n\n", Word);
			}
			else
			{
				fseek(fp, -1, 1);
				printf("%c\t(4,3)\n\n", Word[0]);
			}
			break;
		case'!':
		    printf("%s\t(7,1)\n\n",Word);
			break;
		case'&':
		    printf("%s\t(7,2)\n\n",Word);
			break;
		case'|':
            printf("%s\t(7,3)\n\n",Word);
			break;
		case'\\':
		    ch = fgetc(fp);
		    printf("\\%c\t(8,1)\n\n",ch);
		    break;
		case'%':
		    ch = fgetc(fp);
			if(Isnumber(ch))
			printf("%s\t(3,5)\n\n",Word);
			else
		    printf("%%c\t(9,1)\n\n",ch);
			break;
		case'#':
			printf("%s\t(10,1)\n\n",Word);
			break;
		}
	}
}
int main()
{
	FILE* fp;
	fp = fopen("C:\\Users\\13666\\Desktop\\������ҵ\\test6.txt", "r");
		if (fp == NULL)
	{
		printf("�ļ���ʧ��");
		return -1;
	}
	else
	do {
		ch = fgetc(fp);
		if (ch == '$')
		{
			break;
		}
		else if (ch == ' ' || ch == '\t' || ch == '\n')
		{
		}
		else {
			fseek(fp, -1, 1);     //fpָ���ļ���λ�ú���һ���ַ�
			scan(fp);
		}
	} while (ch != '$');//��$��Ϊ�ļ���ֹ����
	return 0;
}

