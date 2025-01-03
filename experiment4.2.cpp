#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<stack>
using namespace std;
const int Maxv = 200;
const int Max = 100; 
char Vn[Maxv]; 
char Vt[Max]; 
int num_ll_1 = 3;//文法条数
char frist_vt[Max][Max];
char last_vt[Max][Max];
char relation_table[Max][Max];
char input[500]={0};
void openfile();
int whether_Vn(char c);
int whether_Vt(char c);
void add(char ll_1[Max][Max]);
void print_result();
int check_fristvt(char c, int i);
int check_lastvt(char c, int i);
void find_fristvt(char ll_1[Max][Max]);
void find_lastvt(char ll_1[Max][Max]);
void analysis();
char gram[Max][Max] = 
{
  "E->E+T|E-T|T",
  "T->T*F|T/F|F",
  "F->(E)|i"
};

void openfile() {
    FILE* fp;
    int j = 0;
    char str[300];
    if ((fp = fopen("D:\\bianyizuoye\\tuples4.2.txt", "r")) != NULL) 
    {
        while (fgets(str, 300, fp) != NULL) 
        {
            // 移除行尾的换行符
            str[strcspn(str, "\n")] = 0;
            // 找到第一个空格后的字符
            char* space = strchr(str, ' ');
            if (space != NULL) 
            {
                // 跳过空格，获取下一个字符
                char nextChar = *(space + 1);              
                // 将字符追加到input末尾
                int inputLen = strlen(input);
                input[inputLen] = nextChar;
                input[inputLen + 1] = '\0';
            }
        }
        fclose(fp);
    } 
    else 
    {
        cout << "无法打开文件" << endl;
        return;
    }

    // 在input末尾添加结束标记
    int inputLen = strlen(input);
    input[inputLen] = '\0';
    cout << "input sentence:" << input << endl;
}
void add(char ll_1[Max][Max]) 
{
    int Vn_data = 0;
    int Vt_data = 0;
    int temp = 0;
    int temp1 = 0;
    for (int i = 0; i < num_ll_1; i++) 
    {//添加非终结符号
        Vn[Vn_data] = ll_1[i][0];
        Vn_data++;
    }
    for (int i = 0; i < num_ll_1; i++) 
    {
        for (int j = 3; j < strlen(ll_1[i]); j++) 
        {
            //文法开始前三个字符不为终结符号
            temp = 0;
            for (int k = 0; k < strlen(Vn); k++) 
            {
                if (ll_1[i][j] == Vn[k] || ll_1[i][j] == ' ' || ll_1[i][j] == '|') 
                {
                    temp = 1;
                    break;
                }
            }
            if (temp == 0)
             {
                temp1 = 0;
                for (int k = 0; k < strlen(Vt); k++)
                 {
                    //判断该非终结符号是否已经存在
                    if (ll_1[i][j] == Vt[k]) 
                    {
                        temp1 = 1;
                        break;
                    }
                }
                if (temp1 == 0)
                 {
                    Vt[Vt_data] = ll_1[i][j];//添加终结符号
                    Vt_data++;
                }
            }
        }
    }
    Vt[strlen(Vt)] = '#';
}
int whether_Vn(char c) 
{
    int location = -1;
    for (int i = 0; i < num_ll_1; i++) 
    {
        if (c == Vn[i]) 
        {
            location = i;
            break;
        }
    }
    return location;
}
int whether_Vt(char c)
 {
    int location = -1;
    for (int i = 0; i < strlen(Vt); i++) 
    {
        if (c == Vt[i]) 
        {
            location = i;
            break;
        }
    }
    return location;
}
void find_fristvt(char ll_1[Max][Max]) 
{
    int temp;
    for (int i = 0; i < num_ll_1; i++) 
    {
        temp = 1;
        for (int j = 3; j < strlen(ll_1[i]); j++) 
        {
            if (temp == 1) {
                if (whether_Vt(ll_1[i][j]) != -1) 
                {
                    //当前文法的第一个字符为终结符号
                    if (check_fristvt(ll_1[i][j], i) == -1) 
                    {
                        //如果该fristvt集没有该符号
                        frist_vt[i][strlen(frist_vt[i])] = ll_1[i][j];
                        //添加该非终结符号到Fristvt集
                    }
                }
                else 
                {
                    //当前文法的第一个符号不是终结符号
                    if (j != strlen(ll_1[i]) - 1 && ll_1[i][j + 1] != '|') 
                    {
                        //如果当前文法还没结束且不是单产生式子
                        if (check_fristvt(ll_1[i][j + 1], i) == -1) 
                        {
                            frist_vt[i][strlen(frist_vt[i])] = ll_1[i][j + 1];
                        }
                    }
                }
            }
            temp = 0;
            if (ll_1[i][j] == '|') 
            {
                temp = 1;//frist集只判断或符号的后面的句子即可
            }
        }
    }
     //反向遍历按照第二条规则添加fristvt集
        for (int i = num_ll_1 - 1; i >= 0; i--) 
        {
            temp = 1;
            for (int j = 3; j < strlen(ll_1[i]); j++) 
            {
                if (temp == 1) {
                    if (whether_Vn(ll_1[i][j]) != -1) 
                    {
                        //文法当前符号为非终结符号
                        for (int k = 0; k < strlen(frist_vt[whether_Vn(ll_1[i][j])]); k++) 
                        {
                            if (check_fristvt(frist_vt[whether_Vn(ll_1[i][j])][k], i) == -1) 
                            {
                                //如果该fristvt集没有该符号
                                frist_vt[i][strlen(frist_vt[i])] = frist_vt[whether_Vn(ll_1[i][j])][k];
                                //添加该非终结符号到Fristvt集
                            }
                        }
                    }
                }
                temp = 0;
                if (ll_1[i][j] == '|') 
                {
                    temp = 1;
                }
            }
        }
}
void find_lastvt(char ll_1[Max][Max]) 
{
    for (int i = 0; i < num_ll_1; i++) 
    {
        for (int j = 3; j < strlen(ll_1[i]); j++) 
        {
            if (j == strlen(ll_1[i]) - 1 || ll_1[i][j + 1] == '|') 
            {
                if (whether_Vt(ll_1[i][j]) != -1) 
                {
                    //当前文法的字符为终结符号
                    if (check_lastvt(ll_1[i][j], i) == -1) 
                    {
                        //如果该lastvt集没有该符号
                        last_vt[i][strlen(last_vt[i])] = ll_1[i][j];
                        //添加该非终结符号到lastvt集
                    }
                }
                else 
                {
                    //当前文法的符号不是终结符号
                    if (whether_Vt(ll_1[i][j - 1]) != -1) 
                    {
                        //该非终结符号前面是终结符号
                        if (check_lastvt(ll_1[i][j - 1], i) == -1) 
                        {
                            last_vt[i][strlen(last_vt[i])] = ll_1[i][j - 1];
                        }
                    }
                }
            }
        }
    }
    //反向遍历按照第二条规则添加lastvt集
        for (int i = num_ll_1 - 1; i >= 0; i--) 
        {
            for (int j = 3; j < strlen(ll_1[i]); j++) {
                if (j == strlen(ll_1[i]) - 1 || ll_1[i][j + 1] == '|') 
                {
                    if (whether_Vn(ll_1[i][j]) != -1) 
                    {
                        //文法当前符号为非终结符号
                        for (int k = 0; k < strlen(last_vt[whether_Vn(ll_1[i][j])]); k++) 
                        {
                            if (check_lastvt(last_vt[whether_Vn(ll_1[i][j])][k], i) == -1) 
                            {
                                last_vt[i][strlen(last_vt[i])] = last_vt[whether_Vn(ll_1[i][j])][k];
                            }
                        }
                    }
                }
            }
        }
    
}
int check_fristvt(char c, int i) 
{
    int temp = -1;
    for (int j = 0; j < strlen(frist_vt[i]); j++) 
    {
        if (frist_vt[i][j] == c) 
        {
            temp = j;
            break;
        }
    }
    return temp;
}
int check_lastvt(char c, int i) 
{
    int temp = -1;
    for (int j = 0; j < strlen(last_vt[i]); j++) 
    {
        if (last_vt[i][j] == c)
        {
            temp = j;
            break;
        }
    }
    return temp;
}
void make_relation_table(char ll_1[Max][Max]) 
{
    char ll[Max][Max];
    //添加开始符号与结束符号
    for (int i = 1; i < num_ll_1 + 1; i++) 
    {
        strcpy(ll[i], ll_1[i - 1]);
    }
    strcpy(ll[0], "S->#E#");
    //初始化
    for (int i = 0; i < strlen(Vt); i++) 
    {
        for (int j = 0; j < strlen(Vt); j++) 
        {
            relation_table[i][j] = ' ';
        }
    }
    for (int i = 0; i < num_ll_1 + 1; i++) 
    {
        for (int j = 3; j < strlen(ll[i]); j++) 
        {
            //等于
            if (whether_Vn(ll[i][j]) == -1 && ll[i][j] != '|') 
            {
                //该符号为终结符号
                if (j < strlen(ll[i]) - 1) {
                    if (whether_Vn(ll[i][j + 1]) == -1 && ll[i][j] != '|') 
                    {
                        //如果两个终结符号相连,优先关系相等
                        relation_table[whether_Vt(ll[i][j])][whether_Vt(ll[i][j + 1])] = '=';
                    }
                }
                if (j < strlen(ll[i]) - 2) 
                {
                    if (whether_Vn(ll[i][j + 1]) != -1 && ll[i][j + 2] != '|' && whether_Vn(ll[i][j + 2]) == -1) 
                    {
                        //如果两个终结符号中间有一个非终结符，优先关系相等
                        relation_table[whether_Vt(ll[i][j])][whether_Vt(ll[i][j + 2])] = '=';
                    }
                }
            }
            if (j < strlen(ll[i]) - 1) 
            {
                //小于
                if (whether_Vn(ll[i][j]) == -1 && whether_Vn(ll[i][j + 1]) != -1 && ll[i][j] != '|') 
                {
                    //当前字符为终结符号且后面跟着非终结符号
                    for (int k = 0; k < strlen(frist_vt[whether_Vn(ll[i][j + 1])]); k++) 
                    {
                        relation_table[whether_Vt(ll[i][j])][whether_Vt(frist_vt[whether_Vn(ll[i][j + 1])][k])] = '<';
                    }
                }
                //大于
                else if (whether_Vn(ll[i][j]) != -1 && whether_Vn(ll[i][j + 1]) == -1 && ll[i][j + 1] != '|') 
                {
                    //当前字符为非终结符且后面跟着终结符
                    for (int k = 0; k < strlen(last_vt[whether_Vn(ll[i][j])]); k++) 
                    {
                        relation_table[whether_Vt(last_vt[whether_Vn(ll[i][j])][k])][whether_Vt(ll[i][j + 1])] = '>';
                    }
                }
            }
        }
    }
}
void print_result() 
{
    cout << "Fristvt:" << endl;
    for (int i = 0; i < num_ll_1; i++) 
    {
        cout << Vn[i] << ": " << frist_vt[i];
        cout << "  ";
    }
    cout<<endl;
    cout << "Lastvt:" << endl;
    for (int i = 0; i < num_ll_1; i++) 
    {
        cout << Vn[i] << ": " << last_vt[i];
        cout<<"  ";
    }
    cout<<endl;
    cout << "proior matrix:" << endl;
    cout << " ";
    for (int i = 0; i < strlen(Vt); i++) 
    {
        cout << "\t" << Vt[i];
    }
    cout << endl;
    for (int i = 0; i < strlen(Vt); i++) 
    {
        cout << Vt[i];
        for (int j = 0; j < strlen(Vt); j++) 
        {
            cout << "\t" << relation_table[i][j];
        }
        cout << endl;
    }
}
void analysis() 
{
    cout << "   stack" << "           " << "input" << endl;
    char a1, a2;
    int index = 0;
    char stack[100];//分析栈
    char str[100];//输入语句
    int j;
    char c;
    int lengh;
    int temp = 1;
    stack[index] = '#';
    stack[1] = '\0';
    for (int i = 0; i < strlen(input); i++) 
    {
        temp = 1;
        if (whether_Vn(stack[strlen(stack) - 1]) == -1) 
        {
            j = strlen(stack) - 1;
        }
        else j = strlen(stack) - 2;
        //当前关系为等于，入栈
        if (relation_table[whether_Vt(stack[j])][whether_Vt(input[i])] == '=') 
        {
            index = strlen(stack);
            stack[index] = input[i];
            stack[index + 1] = '\0';
        }
        //低于关系也入栈
        else if (relation_table[whether_Vt(stack[j])][whether_Vt(input[i])] == '<') 
        {
            index = strlen(stack);
            stack[index] =input[i];
            stack[index + 1] = '\0';
        }
        else if (relation_table[whether_Vt(stack[j])][whether_Vt(input[i])] == '>') 
        {
            
            for (int y = 0;; y++)
             {
                for (;;) {
                    c = stack[j];
                    lengh = j;
                    if (j >= 1 && whether_Vn(stack[j - 1]) == -1) 
                    {
                        j--;
                    }
                    else if (j >= 2) 
                    {
                        j = j - 2;
                    }
                    if (relation_table[whether_Vt(stack[j])][whether_Vt(c)] == '<') 
                    {
                        break;
                    }
                }
                stack[j + 1] = '\0';
                index = strlen(stack);
                stack[index] = 'E';
                stack[index + 1] = '\0';
                if (y == 0) {
                    for (int u = i; u < strlen(input); u++) 
                    {
                        str[u - i] = input[u];
                        lengh = u - i + 1;
                    }
                    str[lengh] = '\0';
                    cout.width(10);
                    cout.unsetf(ios::left);
                    cout.fill(' ');
                    cout << stack;
                    cout << "         " << str << endl;
                }
                index = strlen(stack);
                stack[index] =input[i];
                stack[index + 1] = '\0';
                for (int u = i; u < strlen(input); u++) 
                {
                    str[u - i] = input[u];
                    lengh = u - i + 1;
                }
                cout.width(10);
                cout.unsetf(ios::left);
                cout.fill(' ');
                str[lengh] = '\0';
                cout << stack;
                cout << "         " << str << endl;
                temp = 0;
                if (strlen(stack) <= 2) { break; }
                a1 = stack[strlen(stack) - 1];
                if (whether_Vn(stack[strlen(stack) - 2]) == -1) 
                {
                    a2 = stack[strlen(stack) - 2];
                }
                else a2 = stack[strlen(stack) - 3];
                if (relation_table[whether_Vt(a2)][whether_Vt(a1)] == '<' || relation_table[whether_Vt(a2)][whether_Vt(a1)] == '=') 
                {
                    break;
                }
            }
        }
        else 
        {
            cout << "analysis fail!" << endl;
            break;
        }
        if (temp != 0) 
        {
            for (int u = i; u < strlen(input); u++) 
            {
                str[u - i] = input[u];
                lengh = u - i + 1;
            }
            str[lengh] = '\0';
            cout.width(10);
            cout.unsetf(ios::left);
            cout.fill(' ');
            cout << stack;
            cout << "         " << str << endl;
        }
    }
    if (strcmp(stack, "#E#") == 0) 
    {
        cout << "analysis success!" << endl;
    }
}
int main() 
{
    openfile();
    add(gram);
    find_fristvt(gram);
    find_lastvt(gram);
    make_relation_table(gram);
    print_result();
    analysis();
    return 0;
}
