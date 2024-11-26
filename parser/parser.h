#pragma once
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../ast/integer.h"
#include "../ast/boolean.h"
#include "../ast/string.h"
#include "../ast/identifier.h"
#include "../ast/expression_statement.h"
#include "../ast/program.h"
#include "../ast/infix.h"
#include "../ast/prefix.h"
class Parser
{
public:
    enum Precedence
    {
        LOWEST = 0,
        ASSIGN,   // =
        BIT,      // 位运算
        EQUALS,   // == != > < >= <=
        SUM,      // 加减
        PRODUCT,  // 乘除
        PRE_SIGN, // 前缀正号负号
        DOT,      // 小数
    };

    Parser();
    Parser(std::vector<Token>::iterator ptokens); // 构造函数，接受token列表的一个迭代器
    ~Parser();

    // 前缀表达式函数原型定义
    typedef std::shared_ptr<Expression> (Parser::*prefix_parse_fn)(void);
    // 中缀表达式函数原型定义
    typedef std::shared_ptr<Expression> (Parser::*infix_parse_fn)(const std::shared_ptr<Expression> &left);

    void next_token();                    // 读取下一个token
    bool curr_token_is(TokenType ty);     // 判断当前token是否是ty类型
    bool peek_token_is(TokenType ty);     // 判断下一个token是否是ty类型
    bool expect_peek_token(TokenType ty); // 判断下一个token是否是期望类型&读取下一位token
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
    // 中缀
    std::shared_ptr<Expression> parse_infix(const std::shared_ptr<Expression> &left);

    void parse_program();
    std::shared_ptr<Statement> parse_statement();
    std::shared_ptr<ExpressionStatement> parse_expression_statement();

    std::shared_ptr<Program> m_program = nullptr;

    void new_sentence(std::vector<Token>::iterator ptokens, std::vector<Token>::iterator ptokens_end);

private:
    std::vector<Token>::iterator m_ptokens;     // 指向下一个token的迭代器
    std::vector<Token>::iterator m_ptokens_end; // 指向tokens.end()的迭代器
    std::shared_ptr<Lexer> m_lexer;
    Token m_curr;                                  // 当前的token
    Token m_peek;                                  // 下一个token
    std::list<std::string> m_errors;               // 存储错误的列表
    static std::map<TokenType, int> m_precedences; // 一个从运算符TokenType类型到优先级类型的映射
    static std::map<TokenType, prefix_parse_fn> m_prefix_parse_fns;
    static std::map<TokenType, infix_parse_fn> m_infix_parse_fns;
};