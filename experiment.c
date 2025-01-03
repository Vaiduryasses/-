#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
char Word[32];
int ch;//存储识别的单词
int number[1000][100];//常数表,number[][0]存储当前行的二进制位数
char mark[100][5];//标识符
int line = 0, row = 0;
const char *key[10] = { "void","char","int","float","double","if","else","for","do","while"};
int Isletter(char c)//判断是否是字母
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	else return 0;
}
int Isnumber(char c)//判断是否为数字
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
		if (m = strcmp(Word, key[i]) == 0)//区分大小写比较
			return i + 1;//在关键字表中存在
	}
		if (line != 0)//判断是否在标识符表中
		{
			int s = 0;
			while (s < line)
			{
				if (strcmp(Word, mark[s++]) == 0)//存在
				{
					printf("%s\t(6,%d)\n\n", Word, s);
					return 1;//为标识符
				}
			}

		}
		strcpy(mark[line], Word);//不存在则加入到标识符表中
		printf("%s\t(6,%d)\n\n", Word, line + 1);
		line++;
		return 1;

}
int IsDecimal(char *Word) 
{
    if (Word == NULL || *Word == '\0') 
        return 0; // 空字符串不是有效的数字
    int i = 0;
    int hasDecimalPoint = 0; // 检查小数点出现的次数
    // 检查第一个字符是否为正负号
    if (Word[i] == '-') 
        i++; // 跳过负号
    // 检查剩余的字符
    for (; Word[i] != '\0'; i++) 
	{
        if (Word[i] == '.') 
		{
            // 如果已经有小数点，再次出现小数点则不合法
            if (hasDecimalPoint)
			return 0;
            hasDecimalPoint = 1; // 第一次遇到小数点
        } 
		else if (!Isnumber(Word[i]))
           {
			printf("输入有误！");// 如果不是数字，且不是小数点，返回不是数字
            return 0;
			}
    }
    // 如果字符串全是合法的小数，返回1,整数返回2
	if(hasDecimalPoint)
    return 1;
	else return 2;
}
void scan(FILE* fp)//扫描器
{
	char Word[32] = { '\0' };
	char ch;
	int i, c,d;
	ch = fgetc(fp);//获取一个字符
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
			printf("标识符格式错误");
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
	fp = fopen("C:\\Users\\13666\\Desktop\\编译作业\\test6.txt", "r");
		if (fp == NULL)
	{
		printf("文件打开失败");
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
			fseek(fp, -1, 1);     //fp指向文件的位置后退一个字符
			scan(fp);
		}
	} while (ch != '$');//以$作为文件截止符号
	return 0;
}

