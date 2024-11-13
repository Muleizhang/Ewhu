#include <iostream>
#include <stdio.h>
#include <vector>
#include "Token.cpp"

class Lexer
{
private:
    static const char *keywords[9];             // 保留字
    static const char *cal_sign[5];             // 运算符
    static const char *space_word_table[8];     // 界限符
    static const char *relation_calcu_table[7]; // 比较运算符

    const std::string source = "";
    std::vector<Token> tokens; // 保存 Token 的列表
    int start = 0;
    int current = 0;
    int line = 1;

public:
    Lexer(const std::string &source) : source(source) {};
    Lexer() = default;
    std::vector<Token> scan_tokens()
    {
        char inpt = nextChar();

        return tokens;
    }

private:
    char nextChar()
    {
        return source[current];
    }
};