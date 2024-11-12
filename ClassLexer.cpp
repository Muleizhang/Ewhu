#include <iostream>
#include <stdio.h>
#include <cstring>
using namespace std;

class Lexer
{
    const char *keywords[9] = {"main", "int", "float", "return", "while", "break", "continue", "if", "else"}; // 保留字
    const char *cal_sign[5] = {"+", "-", "*", "/", "%"};                                                      // 运算符
    const char *space_word_table[8] = {";", ",", "[", "]", "{", "}", "(", ")"};                               // 界限符
    const char *relation_calcu_table[7] = {"<", "<=", ">", ">=", "=", "=="};                                  // 关系符
    char *filepath = (char *)"test.txt";
    FILE *file;

public:
    Lexer()
    {
        cout << "id=1 关键字;   id=2 标识符;   id=3 常数;   id=4 数学运算符;   id=5 关系运算符;   id=6 界限符" << endl;
        file = freopen(filepath, "r", stdin);
    }
    char read();                                // 读取单个字符
    bool isEmpty(char inpt);                    // 读取字符是否为空
    bool isLetter(char inpt);                   // 读取字符是否为字母
    bool isNumber(char inpt);                   // 读取字符是否为数字
    bool isMathematicalOperator(char inpt);     // 读取字符是否为数学运算符
    bool isRelationalOperator(char inpt);       // 读取字符是否为关系运算符
    bool isDelimiter(char inpt);                // 读取字符是否为界限符
    int findType(char inpt);                    // 判断是读取字符是哪一种类型
    int isKeywords(char *kw);                   // 判断字符串是否是关键字
    void processLetter(char inpt);              // 处理字母
    void processNumber(char inpt);              // 处理数字
    int processRelationalOperator(char inpt);   // 处理关系运算符，返回运算符类型
    int processMathematicalOperator(char inpt); // 处理数学运算符，返回运算符类型
    int processDelimiter(char inpt);            // 处理界限符，返回界限符类型
};
