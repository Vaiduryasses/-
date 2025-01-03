#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <stack>
#include <fstream>
using namespace std;

// 定义数据结构
map<string, vector<string>> rule; // 产生式规则
map<char, set<char>> first; // 非终结符的First集
map<char, set<char>> follow; // 非终结符的Follow集
set<char> Vt; // 终结符集合
set<char> Vn; // 非终结符集合
char start; // 开始符号

// 记录分析表坐标
struct point {
    char vt;
    char vn;
    // 重载 < 操作符，用于在 std::map 中比较
    bool operator<(const point& other) const {
        if (vn == other.vn)
            return vt < other.vt;  // 如果非终结符相同，则按终结符比较
        return vn < other.vn;      // 按非终结符比较
    }
};

// LL(1)分析表
map<point, string> table;

// 记录输入的二元组
typedef struct {
    int code;
    char symbol;
} file_tuple;

vector<file_tuple> file_text; // 输入文件

void make_first(){
    // 初始化first集
    for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
        set<char> t;
        first.insert(make_pair(*it, t));
    }

    // 遍历规则rule，填充first集
    for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
        string left = it->first;
        vector<string> right = it->second;

        for (string production : right) {
            if (Vt.count(production[0]) != 0) {
                first[left[0]].insert(production[0]);  // 终结符直接加入first集
            }
            if (production == " ") {
                first[left[0]].insert(' ');  // 空产生式
            }
        }
    }

    // 循环，直到first集不再变化
    bool changed;
    do {
        changed = false;
        for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
            string left = it->first;
            vector<string> right = it->second;

            for (string production : right) {
                if (Vn.count(production[0]) != 0) {
                    set<char> temp = first[production[0]];
                    temp.erase(' ');  // 去除空串
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
    // 初始化follow集
    for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
        set<char> t;
        if (*it == start) t.insert('#');  // 开始符号的follow集包含#号
        follow.insert(make_pair(*it, t));
    }

    // 遍历规则rule，填充follow集
    for (map<string, vector<string>>::iterator it = rule.begin(); it != rule.end(); it++) {
        string left = it->first;
        vector<string> right = it->second;

        for (string production : right) {
            for (size_t i = 0; i < production.size(); i++) {
                if (Vn.count(production[i]) != 0) {
                    // 找到非终结符B
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

    // 循环，直到follow集不再变化
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
    //纵轴遍历Vn
    for(set<char>::iterator it1 = Vn.begin(); it1 != Vn.end(); it1++) {
        //横轴遍历Vt
        for(set<char>::iterator it2 = Vt.begin(); it2 != Vt.end(); it2++) {
            //结构体t（vn，vt）来记录坐标
            point t;
            t.vn = *it1;
            t.vt = *it2;
            action = "";
            
            //如果it2是it1的first集元素: it2∈first(it1)
            if(first.at(t.vn).count(t.vt) != 0) {
                // 本应有一步动作：查询it2是it1哪一个规则的first集元素
                string v = string(1, t.vn); // 将非终结符转化为字符串
                vector<string> Trule;
                Trule = rule.at(v);
                
                // 剔除空产生式
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
            // 如果it2是it1的follow集元素且ε∈first(it1)
            else if (first.at(t.vn).count(' ') != 0 && follow.at(t.vn).count(t.vt) != 0) {
                action = "pop";
                table.insert(make_pair(t, action));
            }
            else {
                action = "-1"; // 出错
                table.insert(make_pair(t, action));
            }
        }

        // 纵轴添加一个‘#’
        point t1;
        t1.vn = *it1;
        t1.vt = '#';
        // 添加条件：Vn->ε 且 ‘#’ ∈ follow(Vn)
        if (first.at(t1.vn).count(' ') != 0 && follow.at(t1.vn).count(t1.vt) != 0) {
            action = "pop";
            table.insert(make_pair(t1, action));
        } else {
            action = "-1"; // 出错
            table.insert(make_pair(t1, action));
        }
    }
}


bool ll1_parse(const vector<file_tuple>& input) {
    stack<char> parse_stack;
    parse_stack.push('#');  // 栈底的特殊符号
    parse_stack.push(start); // 将开始符号压栈

    int index = 0; // 输入字符串的指针

    while (!parse_stack.empty()) {
        char top = parse_stack.top(); // 栈顶符号
        
        cout << "Stack top: " << top << ", Input symbol: " << input[index].symbol << endl;

        if (top == '#') {
            // 如果栈顶是#，并且输入已经处理完，表示接受输入
            if (index == (input.size()-1)) {
                cout << "Parsing Successful!" << endl;
                return true;
            } else {
                cout << "Parsing Failed! Remaining input." << endl;
                return false;
            }
        }

        if (Vt.count(top)) {
            // 如果栈顶是终结符
            if (top == input[index].symbol) {
                cout << "Matching " << top << endl;
                parse_stack.pop();
                index++; // 消耗一个输入符号
            } else {
                cout << "Parsing Failed! Mismatch: expected " << top << " but found " << input[index].symbol << endl;
                return false;
            }
        } else if (Vn.count(top)) {
            // 如果栈顶是非终结符
            point p = {input[index].symbol, top};
            if (table.count(p) == 0 || table[p] == "-1") {
                cout << "Parsing Failed! No valid production for " << top << " and " << input[index].symbol << endl;
                return false;
            }

            string production = table[p];
            parse_stack.pop();  // 弹出栈顶符号

            if (production != "pop") {
                // 逆序将产生式的右侧压栈
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



// 读取二元组文件并解析
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

        if (symbol == '#') break;  // 读取到结束符号#
    }

    file.close();
}

int main() {
    // 1. 设置文法规则
    rule["S"] = {"V=E"};
    rule["E"] = {"TY"};
    rule["Y"] = {"ATY", " "};
    rule["T"] = {"FZ"};
    rule["Z"] = {"MFZ", " "};
    rule["F"] = {"(E)", "i"};
    rule["A"] = {"+", "-"};
    rule["M"] = {"*", "/"};
    rule["V"] = {"i"};

    // 2. 设置文法符号集合
    Vt = {'+', '-', '*', '/', '=', '(', ')', 'i'};  // 终结符
    Vn = {'S', 'E', 'Y', 'T', 'Z', 'F', 'A', 'M', 'V'}; // 非终结符
    start = 'S'; // 开始符号

    // 3. 构造FIRST集、FOLLOW集和分析表
    make_first();
    make_follow();
    make_table();

    // 4. 读取输入文件（包含二元组）
    string input_file = "D:\\bianyizuoye\\tuples4.1.txt"; // 输入文件名
    read_input_file(input_file);

    // 5. 进行LL(1)语法分析
    ll1_parse(file_text);

    return 0;
}
