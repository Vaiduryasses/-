#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include<set>
using namespace std;

string symbolfier[100];  // ��ʶ����
string morpheme[100];    // ��Ԫ���
int symcounter = 0;      // ��ʶ��������
int morphemecounter = 0; // ��Ԫ�������
int pMorpheme = 0;       // ��ǰ�����Ķ�Ԫ��ָ��

// ��������
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

// ��ȡ��ʶ���ļ�
void readfile_symbolfier() {
    FILE* fp = fopen("C:\\Users\\13666\\Desktop\\������ҵ\\symbol_table.txt", "r");
    if (fp == NULL) {
        cout << "��ʶ���ļ���ʧ��" << endl;
        exit(0);
    }
    int i = 0;
    char temp[100] = "\0";
    while (!feof(fp)) {
        fgets(temp, 100, fp);
        symbolfier[i] = temp;
        if (!symbolfier[i].empty() && symbolfier[i][symbolfier[i].length() - 1] == '\n') 
        {
            symbolfier[i].erase(symbolfier[i].length() - 1); // ȥ��β�����з�
        }
        i++;
    }
    symcounter = i;
    fclose(fp);
}

// ��ȡ��Ԫ���ļ�
void readFile_tuple() {
    FILE* fp = fopen("C:\\Users\\13666\\Desktop\\������ҵ\\tuples1.txt", "r");
    if (fp == NULL) {
        cout << "��Ԫ���ļ���ʧ��" << endl;
        exit(0);
    }
    int i = 0;
    char temp[100] = "\0";
    while (!feof(fp)) {
        fgets(temp, 100, fp);
        morpheme[i] = temp;
        if (!morpheme[i].empty() && morpheme[i][morpheme[i].length() - 1] == '\n') 
        {
            morpheme[i].erase(morpheme[i].length() - 1); // ȥ��β�����з�
        }
        i++;
    }
    morphemecounter = i;
    fclose(fp);
}

// ƥ�亯������鵱ǰ��Ԫ������
int match(int expectedType) 
{
    if (pMorpheme < morphemecounter) 
    {
         int currentType = atoi(morpheme[pMorpheme].substr(3, 1).c_str());  // ��ȡ��Ԫ�������
         if (currentType == expectedType) 
         {
            return 1;
         }

    }
    return 0;
}

// �﷨������
void syntaxError() 
{
    cout << "Syntax error at token " << pMorpheme+1 << endl;//�ڼ�����Ԫ��������
    exit(1);
}

// �ж��Ƿ��� Follow ����
set<int> follow_E_prime ; // E' �� Follow ���� (2,5)Ϊ ')'�� '10' Ϊ '#'
set<int> follow_T_prime; // T' �� Follow ���� (3,1),(3,2)Ϊ '+', '-', (2,5) Ϊ ')'�� '10' Ϊ '#'
// Follow����ʼ������
void initializeFollowSets() {
    // ��ʼ�� follow_E_prime ����
    follow_E_prime.insert(5);
    follow_E_prime.insert(10);

    // ��ʼ�� follow_T_prime ����
    follow_T_prime.insert(1);
    follow_T_prime.insert(2);
    follow_T_prime.insert(5);
    follow_T_prime.insert(10);
}
// S �� V = E
void S() 
{
    V();
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if(currentType==4)
    {
        if (!match(2)) syntaxError();  // '=' ������Ϊ(4, 2)
    }
    else syntaxError();
    pMorpheme++;
    E();
}

// E �� T E'
void E() 
{
    T();
    E_prime();
    
}

// E' �� A T E' | ��
void E_prime()
 {
    if (pMorpheme < morphemecounter) //�ղ���
    {
        int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
        if (currentType == 3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=4) 
        {  // '+' (3,1)�� '-'(3,2)
            A();
            T();
            E_prime();   
        }
        else if (follow_E_prime.find(atoi(morpheme[pMorpheme].substr(3, 1).c_str())) == follow_E_prime.end()) {
            syntaxError();
    }
    }
}

// T �� F T'
void T() 
{
    F();
    T_prime();
    
}

// T' �� M F T' | ��
void T_prime() 
{
    if (pMorpheme < morphemecounter) 
    {
        int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
        if (currentType == 3&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=1&&atoi(morpheme[pMorpheme].substr(3, 1).c_str()) !=2)
        {  // '*'(3,3) �� '/'(3,4)
            M();
            F();
            T_prime();
        }
        else if (follow_T_prime.find(atoi(morpheme[pMorpheme].substr(3, 1).c_str())) == follow_T_prime.end()) {
            syntaxError();
        }
    }

}

// F �� (E) | i
void F() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if (currentType == 2) 
    {  // '('(2,4)
        match(4);
        pMorpheme++;
        E();
        if (!match(5)) syntaxError();  // ')' (2,5)������
        pMorpheme++;
    } 
    else if (currentType == 6) 
    {     // ��ʶ�� 'i' ������
        pMorpheme++;
    }
     else 
    {
        syntaxError();
    }
    
}

// A �� + | -
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

// M �� * | /
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

// V �� i
void V() 
{
    int currentType = atoi(morpheme[pMorpheme].substr(1, 1).c_str());
    if(currentType!=6)
    syntaxError();  // 'i' ������Ϊ 6
    pMorpheme++;
}

// ������
int main() {
    initializeFollowSets(); // ��ʼ�� Follow ��
    readFile_tuple();
    readfile_symbolfier();
    
    S();  // ��ʼ�ݹ��½�����

    if (pMorpheme == morphemecounter) 
    {  // ��������
        cout << "ƥ��ɹ�" << endl;
    } 
    else 
    {
        cout << "ƥ��ʧ��" << endl;
    }

    return 0;
}
