#pragma once
#include <map>
#include <list>
#include <memory>
#include <unordered_map>
#include <sstream>
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../ast/node.h"
#include "../ast/statement.h"
#include "../ast/infix.h"

class Parser
{
public:
    enum Precedence
    {
        LOWEST = 0,
        ASSIGN,      // =
        LOGICAL,     // && || ^
        BIT,         // 位运算
        EQUALS,      // == != > < >= <=
        SUM,         // 加减
        PRODUCT,     // 乘除
        POWER,       // 乘方
        TRIGNOMETRY, // 三角函数
        PRE_SIGN,    // 前缀正号负号
        DOT,         // 小数
        INDEX,       // 索引
    };

    Parser();
    Parser(std::vector<Token>::iterator ptokens); // 构造函数，接受token列表的一个迭代器
    ~Parser();
    static int hash(const std::string &str);
    static constexpr int prehash(const char *str);

    void parse_program(std::vector<Token> &tokens); // 解析程序

public:
    std::shared_ptr<Program> m_program = nullptr;
    std::unordered_map<int, std::string> identifier_map; // 标识符反映射
    std::unordered_map<int, std::string> function_map;   // 函数反映射

private:
    // 前缀表达式函数原型定义
    typedef std::shared_ptr<Expression> (Parser::*prefix_parse_fn)(void);
    // 中缀表达式函数原型定义
    typedef std::shared_ptr<Expression> (Parser::*infix_parse_fn)(const std::shared_ptr<Expression> &left);
    // 后缀表达式函数原型定义
    typedef std::shared_ptr<Expression> (Parser::*suffix_parse_fn)(const std::shared_ptr<Expression> &left);
    // 控制流语句函数原型定义
    typedef std::shared_ptr<Statement> (Parser::*control_flow_fn)(void);

    void new_sentence(std::vector<Token>::iterator ptokens, std::vector<Token>::iterator ptokens_end);
    std::shared_ptr<Statement> parse_statement();

    // 函数
    std::shared_ptr<Statement> parse_function_declaration();
    std::shared_ptr<ExpressionStatement> parse_expression_statement();

    void next_token();                    // 读取下一个token
    bool curr_token_is(TokenType ty);     // 判断当前token是否是ty类型
    bool peek_token_is(TokenType ty);     // 判断下一个token是否是ty类型
    void expect_peek_token(TokenType ty); // 判断下一个token是否是期望类型&读取下一位token
    void peek_error(TokenType type);      // 下一个token错误

    int curr_token_precedence(); // 返回当前token的优先级
    int peek_token_precedence(); // 返回下一个token的优先级

    void no_prefix_parse_fn_error(TokenType ty);

    std::list<std::string> &errors();                             // 返回m_errors
    std::shared_ptr<Expression> parse_expression(int precedence); // 处理表达式

    // 前缀
    std::shared_ptr<Expression> parse_integer();
    std::shared_ptr<Expression> parse_string();
    std::shared_ptr<Expression> parse_boolean();
    std::shared_ptr<Expression> parse_group();
    std::shared_ptr<Expression> parse_prefix();
    std::shared_ptr<Expression> parse_identifier();
    std::shared_ptr<Expression> parse_identifier_function();
    std::shared_ptr<Expression> parse_array();
    std::shared_ptr<Expression> parse_trignometry();
    // 中缀
    std::shared_ptr<Expression> parse_infix(const std::shared_ptr<Expression> &left);
    std::shared_ptr<Expression> parse_index(const std::shared_ptr<Expression> &left);

    // 后缀
    // std::shared_ptr<Expression> parse_suffix(const std::shared_ptr<Expression> &left){};

    // 控制流
    std::shared_ptr<Statement> parse_statement_block(); // 语句块
    std::shared_ptr<Statement> parse_if_statement();    // if语句
    std::shared_ptr<Statement> parse_while_statement(); // while语句
    std::shared_ptr<Statement> parse_break_statement();
    std::shared_ptr<Statement> parse_continue_statement();
    std::shared_ptr<Statement> parse_return_statement();

private:
    std::vector<Token>::iterator m_ptokens;     // 指向下一个token的迭代器
    std::vector<Token>::iterator m_ptokens_end; // 指向tokens.end()的迭代器
    std::shared_ptr<Lexer> m_lexer;
    Token m_curr;                    // 当前的token
    Token m_peek;                    // 下一个token
    std::list<std::string> m_errors; // 存储错误的列表

    static std::map<TokenType, int> m_precedences; // 一个从运算符TokenType类型到优先级类型的映射
    static std::unordered_map<TokenType, prefix_parse_fn> m_prefix_parse_fns;
    static std::unordered_map<TokenType, infix_parse_fn> m_infix_parse_fns;
    static std::unordered_map<TokenType, suffix_parse_fn> m_suffix_parse_fns;
    static std::unordered_map<TokenType, control_flow_fn> m_control_flow_fns;
};

constexpr int Parser::prehash(const char *str)
{
    int hash = 0;
    while (*str)
    {
        hash = hash * 31 + *str++;
    }
    return hash;
}