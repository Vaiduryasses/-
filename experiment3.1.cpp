#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <stack>
#include <fstream>
using namespace std;

// �������ݽṹ
map<string, vector<string>> rule; // ����ʽ����
map<char, set<char>> first; // ���ս����First��
map<char, set<char>> follow; // ���ս����Follow��
set<char> Vt; // �ս������
set<char> Vn; // ���ս������
char start; // ��ʼ����

// ��¼����������
struct point {
    char vt;
    char vn;
    // ���� < �������������� std::map �бȽ�
    bool operator<(const point& other) const {
        if (vn == other.vn)
            return vt < other.vt;  // ������ս����ͬ�����ս���Ƚ�
        return vn < other.vn;      // �����ս���Ƚ�
    }
};

// LL(1)������
map<point, string> table;

// ��¼����Ķ�Ԫ��
typedef struct {
    int code;
    char symbol;
} file_tuple;

vector<file_tuple> file_text; // �����ļ�

void make_first(){
    // ��ʼ��first��
    for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
        set<char> t;
        first.insert(make_pair(*it, t));
    }

    // ��������rule�����first��
    for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
        string left = it->first;
        vector<string> right = it->second;

        for (string production : right) {
            if (Vt.count(production[0]) != 0) {
                first[left[0]].insert(production[0]);  // �ս��ֱ�Ӽ���first��
            }
            if (production == " ") {
                first[left[0]].insert(' ');  // �ղ���ʽ
            }
        }
    }

    // ѭ����ֱ��first�����ٱ仯
    bool changed;
    do {
        changed = false;
        for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
            string left = it->first;
            vector<string> right = it->second;

            for (string production : right) {
                if (Vn.count(production[0]) != 0) {
                    set<char> temp = first[production[0]];
                    temp.erase(' ');  // ȥ���մ�
                    size_t old_size = first[left[0]].size();
                    first[left[0]].insert(temp.begin(), temp.end());
                    if (first[left[0]].size() > old_size) {
                        changed = true;
                    }
                }
            }
        }
    } while (changed);
}

void make_follow() {
    // ��ʼ��follow��
    for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
        set<char> t;
        if (*it == start) t.insert('#');  // ��ʼ���ŵ�follow������#��
        follow.insert(make_pair(*it, t));
    }

    // ��������rule�����follow��
    for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
        string left = it->first;
        vector<string> right = it->second;

        for (string production : right) {
            for (size_t i = 0; i < production.size(); i++) {
                if (Vn.count(production[i]) != 0) {
                    // �ҵ����ս��B
                    set<char> temp;
                    if (i + 1 < production.size() && Vt.count(production[i + 1]) != 0) {
                        temp.insert(production[i + 1]);
                        follow[production[i]].insert(temp.begin(), temp.end());
                    }
                    if (i + 1 < production.size() && Vn.count(production[i + 1]) != 0) {
                        set<char> temp_follow = first[production[i + 1]];
                        temp_follow.erase(' ');
                        follow[production[i]].insert(temp_follow.begin(), temp_follow.end());
                    }
                    if (i == production.size() - 1) {
                        set<char> temp_follow = follow[left[0]];
                        follow[production[i]].insert(temp_follow.begin(), temp_follow.end());
                    }
                }
            }
        }
    }

    // ѭ����ֱ��follow�����ٱ仯
    bool changed;
    do {
        changed = false;
        for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
            string left = it->first;
            vector<string> right = it->second;

            for (string production : right) {
                for (size_t i = 0; i < production.size(); i++) {
                    if (Vn.count(production[i]) != 0) {
                        set<char> temp = follow[left[0]];
                        size_t old_size = follow[production[i]].size();
                        follow[production[i]].insert(temp.begin(), temp.end());
                        if (follow[production[i]].size() > old_size) {
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}

void make_table() {
    string action;
    //�������Vn
    for(set<char>::iterator it1 = Vn.begin(); it1 != Vn.end(); it1++) {
        //�������Vt
        for(set<char>::iterator it2 = Vt.begin(); it2 != Vt.end(); it2++) {
            //�ṹ��t��vn��vt������¼����
            point t;
            t.vn = *it1;
            t.vt = *it2;
            action = "";
            
            //���it2��it1��first��Ԫ��: it2��first(it1)
            if(first.at(t.vn).count(t.vt) != 0) {
                // ��Ӧ��һ����������ѯit2��it1��һ�������first��Ԫ��
                string v = string(1, t.vn); // �����ս��ת��Ϊ�ַ���
                vector<string> Trule;
                Trule = rule.at(v);
                
                // �޳��ղ���ʽ
                int T = 0;
                for (vector<string>::iterator it = Trule.begin(); T < Trule.size(); it++, T++) {
                    if (*it == " ") {
                        Trule.erase(it);
                    }
                }
                
                if (Trule.size() > 1) {
                    for (int ii = 0; ii < Trule.size(); ii++) {
                        if (Trule[ii] == " ") continue;
                        if (Trule[ii].find(t.vt) != string::npos) {
                            action = Trule[ii];
                            break;
                        }
                    }
                } else {
                    action = rule.at(v)[0];
                }
                table.insert(make_pair(t, action));
            }
            // ���it2��it1��follow��Ԫ���Ҧš�first(it1)
            else if (first.at(t.vn).count(' ') != 0 && follow.at(t.vn).count(t.vt) != 0) {
                action = "pop";
                table.insert(make_pair(t, action));
            }
            else {
                action = "-1"; // ����
                table.insert(make_pair(t, action));
            }
        }

        // �������һ����#��
        point t1;
        t1.vn = *it1;
        t1.vt = '#';
        // ���������Vn->�� �� ��#�� �� follow(Vn)
        if (first.at(t1.vn).count(' ') != 0 && follow.at(t1.vn).count(t1.vt) != 0) {
            action = "pop";
            table.insert(make_pair(t1, action));
        } else {
            action = "-1"; // ����
            table.insert(make_pair(t1, action));
        }
    }
}


bool ll1_parse(const vector<file_tuple>& input) {
    stack<char> parse_stack;
    parse_stack.push('#');  // ջ�׵��������
    parse_stack.push(start); // ����ʼ����ѹջ

    int index = 0; // �����ַ�����ָ��

    while (!parse_stack.empty()) {
        char top = parse_stack.top(); // ջ������
        
        cout << "Stack top: " << top << ", Input symbol: " << input[index].symbol << endl;

        if (top == '#') {
            // ���ջ����#�����������Ѿ������꣬��ʾ��������
            if (index == (input.size()-1)) {
                cout << "Parsing Successful!" << endl;
                return true;
            } else {
                cout << "Parsing Failed! Remaining input." << endl;
                return false;
            }
        }

        if (Vt.count(top)) {
            // ���ջ�����ս��
            if (top == input[index].symbol) {
                cout << "Matching " << top << endl;
                parse_stack.pop();
                index++; // ����һ���������
            } else {
                cout << "Parsing Failed! Mismatch: expected " << top << " but found " << input[index].symbol << endl;
                return false;
            }
        } else if (Vn.count(top)) {
            // ���ջ���Ƿ��ս��
            point p = {input[index].symbol, top};
            if (table.count(p) == 0 || table[p] == "-1") {
                cout << "Parsing Failed! No valid production for " << top << " and " << input[index].symbol << endl;
                return false;
            }

            string production = table[p];
            parse_stack.pop();  // ����ջ������

            if (production != "pop") {
                // ���򽫲���ʽ���Ҳ�ѹջ
                for (int i = production.size() - 1; i >= 0; i--) {
                    if (production[i] != ' ') {
                        parse_stack.push(production[i]);
                    }
                }
            }
            cout << "Applying production: " << top << " -> " << production << endl;
        } else {
            cout << "Parsing Failed! Unexpected symbol: " << top << endl;
            return false;
        }
    }
    return true;
}



// ��ȡ��Ԫ���ļ�������
void read_input_file(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening input file!" << endl;
        exit(1);
    }

    int code;
    char symbol;
    while (file >> code >> symbol) {
        file_tuple t;
        t.code = code;
        t.symbol = symbol;
        file_text.push_back(t);

        if (symbol == '#') break;  // ��ȡ����������#
    }

    file.close();
}

int main() {
    // 1. �����ķ�����
    rule["S"] = {"V=E"};
    rule["E"] = {"TY"};
    rule["Y"] = {"ATY", " "};
    rule["T"] = {"FZ"};
    rule["Z"] = {"MFZ", " "};
    rule["F"] = {"(E)", "i"};
    rule["A"] = {"+", "-"};
    rule["M"] = {"*", "/"};
    rule["V"] = {"i"};

    // 2. �����ķ����ż���
    Vt = {'+', '-', '*', '/', '=', '(', ')', 'i'};  // �ս��
    Vn = {'S', 'E', 'Y', 'T', 'Z', 'F', 'A', 'M', 'V'}; // ���ս��
    start = 'S'; // ��ʼ����

    // 3. ����FIRST����FOLLOW���ͷ�����
    make_first();
    make_follow();
    make_table();

    // 4. ��ȡ�����ļ���������Ԫ�飩
    string input_file = "D:\\bianyizuoye\\tuples4.1.txt"; // �����ļ���
    read_input_file(input_file);

    // 5. ����LL(1)�﷨����
    ll1_parse(file_text);

    return 0;
}
