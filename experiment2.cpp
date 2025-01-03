#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include<set>
using namespace std;

string symbolfier[100];  // 标识符表
string morpheme[100];    // 二元组表
int symcounter = 0;      // 标识符计数器
int morphemecounter = 0; // 二元组计数器
int pMorpheme = 0;       // 当前解析的二元组指针

// 函数声明
void S();
void E();
void E_prime();
void T();
void T_prime();
void F();
void A();
void M();
void V();
void readfile_symbolfier();
void readFile_tuple();
int match(int expectedType);
void syntaxError();
void initializeFollowSets() ;

// 读取标识符文件
void readfile_symbolfier() {
    FILE* fp = fopen("C:\\Users\\13666\\Desktop\\编译作业\\symbol_table.txt", "r");
    if (fp == NULL) {
        cout << "标识符文件打开失败" << endl;
        exit(0);
    }
    int i = 0;
    char temp[100] = "\0";
    while (!feof(fp)) {
        fgets(temp, 100, fp);
        symbolfier[i] = temp;
        if (!symbolfier[i].empty() && symbolfier[i][symbolfier[i].length() - 1] == '\n') 
        {
            symbolfier[i].erase(symbolfier[i].length() - 1); // 去掉尾部换行符
        }
        i++;
    }
    symcounter = i;
    fclose(fp);
}

// 读取二元组文件
void readFile_tuple() {
    FILE* fp = fopen("C:\\Users\\13666\\Desktop\\编译作业\\tuples1.txt", "r");
    if (fp == NULL) {
        cout << "二元组文件打开失败" << endl;
        exit(0);
    }
    int i = 0;
    char temp[100] = "\0";
    while (!feof(fp)) {
        fgets(temp, 100, fp);
        morpheme[i] = temp;
        if (!morpheme[i].empty() && morpheme[i][morpheme[i].length() - 1] == '\n') 
        {
            morpheme[i].erase(morpheme[i].length() - 1); // 去掉尾部换行符
        }
        i++;
    }
    morphemecounter = i;
    fclose(fp);
}

// 匹配函数：检查当前二元组类型
int match(int expectedType) 
{
    if (pMorpheme < morphemecounter) 
    {
         int currentType = atoi(morpheme[pMorpheme].substr(3, 1).c_str());  // 获取二元组的类型
         if (currentType == expectedType) 
         {
            return 1;
         }

    }
    return 0;
}

// 语法错误处理
void syntaxError() 
{
    cout << "Syntax error at token " << pMorpheme+1 << endl;//第几个二元组有问题
    exit(1);
}

// 判断是否在 Follow 集中
set<int> follow_E_prime ; // E' 的 Follow 集， (2,5)为 ')'， '10' 为 '#'
set<int> follow_T_prime; // T' 的 Follow 集， (3,1),(3,2)为 '+', '-', (2,5) 为 ')'， '10' 为 '#'
// Follow集初始化函数
void initializeFollowSets() {
    // 初始化 follow_E_prime 集合
    follow_E_prime.insert(5);
    follow_E_prime.insert(10);

    // 初始化 follow_T_prime 集合
    follow_T_prime.insert(1);
    follow_T_prime.insert(2);
    follow_T_prime.insert(5);
    follow_T_prime.insert(10);
}
// S → V = E
void S() 
{
    V();
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if(currentType==4)
    {
        if (!match(2)) syntaxError();  // '=' 的类型为(4, 2)
    }
    else syntaxError();
    pMorpheme++;
    E();
}

// E → T E'
void E() 
{
    T();
    E_prime();
    
}

// E' → A T E' | ε
void E_prime()
 {
    if (pMorpheme < morphemecounter) //空操作
    {
        int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
        if (currentType == 3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=4) 
        {  // '+' (3,1)或 '-'(3,2)
            A();
            T();
            E_prime();   
        }
        else if (follow_E_prime.find(atoi(morpheme[pMorpheme].substr(3, 1).c_str())) == follow_E_prime.end()) {
            syntaxError();
    }
    }
}

// T → F T'
void T() 
{
    F();
    T_prime();
    
}

// T' → M F T' | ε
void T_prime() 
{
    if (pMorpheme < morphemecounter) 
    {
        int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
        if (currentType == 3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=1&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=2)
        {  // '*'(3,3) 或 '/'(3,4)
            M();
            F();
            T_prime();
        }
        else if (follow_T_prime.find(atoi(morpheme[pMorpheme].substr(3, 1).c_str())) == follow_T_prime.end()) {
            syntaxError();
        }
    }

}

// F → (E) | i
void F() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if (currentType == 2) 
    {  // '('(2,4)
        match(4);
        pMorpheme++;
        E();
        if (!match(5)) syntaxError();  // ')' (2,5)的类型
        pMorpheme++;
    } 
    else if (currentType == 6) 
    {     // 标识符 'i' 的类型
        pMorpheme++;
    }
     else 
    {
        syntaxError();
    }
    
}

// A → + | -
void A() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if (currentType == 3) 
    {      // '+'"-"
        if(!(match(1)||match(2)))syntaxError();
    } 
   
    else 
    {
        syntaxError();
    }
    pMorpheme++;
}

// M → * | /
void M() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if (currentType == 3)
     {         // '*'"/"
       if(!(match(3)||match(4)))syntaxError();
    } 
    else 
    {
        syntaxError();
    }
    pMorpheme++;
}

// V → i
void V() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if(currentType!=6)
    syntaxError();  // 'i' 的类型为 6
    pMorpheme++;
}

// 主函数
int main() {
    initializeFollowSets(); // 初始化 Follow 集
    readFile_tuple();
    readfile_symbolfier();
    
    S();  // 开始递归下降解析

    if (pMorpheme == morphemecounter) 
    {  // 完整解析
        cout << "匹配成功" << endl;
    } 
    else 
    {
        cout << "匹配失败" << endl;
    }

    return 0;
}
