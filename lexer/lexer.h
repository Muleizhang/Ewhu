#pragma once
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "token.h"

class Lexer
{
public:
    static const char *keywords[9];             // 保留字
    static const char *cal_sign[7];             // 运算符
    static const char *space_word_table[8];     // 界限符
    static const char *relation_calcu_table[7]; // 比较运算符

    std::string source;
    std::vector<Token> tokens; // 还未成句的 Token 列表
    int braceStatus = 0;

    Lexer() {}
    Lexer(const std::string &source) : source(source) {}

    // 返回下一个Token
    Token nextToken();
    // 读取一行代码，返回 句子 的列表
    std::vector<std::vector<Token>> scanTokens();

private:
    int start = 0;
    int current = 0;
    int line = 1;
    int read_current = 0;
    int bracketStatus = 0; // := 前括号数量 - 后括号数量
    std::vector<std::vector<Token>> sentences;

    void scanToken(char inpt);                  // 读取一个 Token
    char nextChar();                            // 读取下一个字符
    void addToken(TokenType type);              // 添加 Token
    bool isEmpty(char inpt);                    // 读取字符是否为空
    bool isLetter(char inpt);                   // 读取字符是否为字母
    bool isNumber(char inpt);                   // 读取字符是否为数字
    bool isMathematicalOperator(char inpt);     // 读取字符是否为数学运算符
    bool isRelationalOperator(char inpt);       // 读取字符是否为关系运算符
    bool isDelimiter(char inpt);                // 读取字符是否为界限符
    int findType(char inpt);                    // 判断是读取字符是哪一种类型
    int isKeywords(char *kw);                   // 判断字符串是否是关键字
    Token tokenLetter(char inpt);               // 读取字母
    Token tokenNumber(char inpt);               // 读取数字
    Token tokenString();                        // 读取字符串
    void processLetter(char inpt);              // 处理字母
    void processNumber(char inpt);              // 处理数字
    int processRelationalOperator(char inpt);   // 处理关系运算符，返回运算符类型
    int processMathematicalOperator(char inpt); // 处理数学运算符，返回运算符类型
    int processDelimiter(char inpt);            // 处理界限符，返回界限符类型
};
