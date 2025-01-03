# -
# bjtu  2024 编译实验

本项目包含多个专题，分别围绕编译原理各阶段（词法分析、语法分析、语义分析、中间代码生成等）进行实践。各专题内容和要求如下：

---

## 专题目录

1. **专题 1：词法分析程序设计原理与实现技术**  
2. **专题 2：递归下降语法分析设计原理与实现技术（词法分析 + 语法分析）**  
3. **专题 3：LL(1)语法分析设计原理与实现技术（词法分析 + 语法分析）**  
4. **专题 4：算符优先语法分析设计原理与实现技术（词法分析 + 语法分析）**  
5. **专题 5：基于 SLR(1)分析的语义分析及中间代码生成设计原理与实现技术（编译器前端）**

---

## 词法分析标准

> 以下标准主要适用于**实验一**和**实验二**中使用的词法分析器。

### 1. 关键字（种别码：1）
- `void` → 1  
- `char` → 2  
- `int` → 3  
- `float` → 4  
- `double` → 5  
- `if` → 6  
- `else` → 7  
- `for` → 8  
- `do` → 9  
- `while` → 10  

### 2. 分界符（种别码：2）
- `,` → 0  
- `;` → 1  
- `.` → 2  
- `:` → 3 （`:=` 也为 3）  
- `(` → 4  
- `)` → 5  

### 3. 算术运算符（种别码：3）
- `+` → 1  
- `-` → 2  
- `*` → 3  
- `/` → 4  
- `%` → 5  

### 4. 关系运算符（种别码：4）
- `<` → 0  
- `<=` → 1  
- `=` → 2  
- `>` → 3  
- `>=` → 4  
- `<>` → 5   (表示不等于)

### 5. 常数（种别码：5）
- 1 表示小数  
- 2 表示整数  

### 6. 标识符（种别码：6）
- 从 1 开始增加（例如，识别到的第一个标识符记为 1，第二个为 2，依次类推）

### 7. 逻辑运算符（种别码：7）
- `!` → 1  
- `&` → 2  
- `|` → 3  

### 8. 转义字符（种别码：8）
- 例如 `\n`, `\t`, `\\` 等

### 9. 格式字符（种别码：9）
- 例如 `"%d"`, `"%f"`, `"%s"` 等

---

## 后续三个实验词法分析输入举例（便于编程需要）

示例输入（行首数字仅做参考，程序实际只识别后面字符）：

10 i

19 +


10 i


24 /


27 (


9 i


28 )


-1 #


- 由于程序只识别后面的字符，前面的数字相当于“偷懒”的标号或提示，不影响词法分析实际处理。  
- 当遇到 `-1 #` 等符号时，一般表示输入结束。

---

## 参考资料

- 编译原理相关教材、讲义  
- [MoYangSensei 的 LL(1) 语法分析介绍](https://moyangsensei.github.io/2019/05/20/%E7%BC%96%E8%AF%91%E5%8E%9F%E7%90%86%EF%BC%9ALL(1)%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90/)   
- CSDN
---

## 使用说明

1. **环境要求**  
   - C/C++   
   
---

## 备注（请自行编写输入文件）

- 本作业中，除题目所列的关键字、运算符与分界符外，如需额外处理字符串、注释或更完整的错误恢复机制，可自行扩展。
- 后续实验需要在前面实验（词法分析）的基础上，结合其他阶段（语法、语义、中间代码生成）完成更完整的编译器前端。
- 实验代码并不完善，可能存在问题，请各位学弟学妹谨慎使用。

