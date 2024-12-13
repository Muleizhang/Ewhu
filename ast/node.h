#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
#include "../lexer/token.h"
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"

class Identifier;
class StatementBlock;
class Function;
class ExpressionStatement;
class Statement;
class Expression;

class Node
{
public:
    enum Type
    {
        NODE_INTEGER = 0,
        NODE_FLOAT,
        NODE_STRING,
        NODE_BOOLEAN,
        NODE_INFIX,
        NODE_PREFIX,
        NODE_IDENTIFIER,
        NODE_EXPRESSION_STATEMENT,
        NODE_PROGRAM,
        NODE_COMMENT,
        NODE_STATEMENTBLOCK,
        NODE_IFSTATEMENT,
        NODE_WHILESTATEMENT,
        NODE_BREAKSTATEMENT,
        NODE_CONTINUESTATEMENT,
        NODE_FUNCTION,            // 声明
        NODE_FUNCTION_IDENTIFIER, // 调用
        NODE_RETURNSTATEMENT,     // 函数返回
        NODE_ARRAY,               // 数组
    };

    Node() {}
    Node(Type type) : m_type(type) {}
    virtual ~Node() {}

    Type type() { return m_type; }
    std::string name() const;
    virtual rapidjson::Value json(rapidjson::Document &father) = 0;

public:
    Type m_type;
    Token m_token;
    static std::unordered_map<Type, std::string> m_names;

public:
    std::vector<std::shared_ptr<Node>> m_statements;

    std::shared_ptr<Identifier> m_func;
    std::shared_ptr<StatementBlock> m_statement;
    std::vector<std::shared_ptr<Node>> m_initial_list;

    // re std::vector<std::shared_ptr<Statement>> m_statements;
    std::vector<std::shared_ptr<Function>> m_functions;

    std::shared_ptr<Expression> m_expression;

    // std::shared_ptr<Expression> m_expression;
    std::shared_ptr<Statement> m_true_statement;
    // std::shared_ptr<Statement> m_false_statement;
    std::shared_ptr<Statement> m_false_statement;

    // std::shared_ptr<Expression> m_expression;
    std::shared_ptr<Statement> m_cycle_statement;
    // std::shared_ptr<Statement> m_false_statement;

    std::shared_ptr<ExpressionStatement> m_expression_statement;

    int m_name = 0;
    long long m_value = 0;
    bool m_bool = false;
    std::string m_string = "";

    TokenType m_operator;                // 运算符
    std::shared_ptr<Expression> m_left;  // 左表达式
    std::shared_ptr<Expression> m_right; // 右表达式
};

class Expression : public Node
{
public:
    Expression() : Node() {}
    Expression(Type type) : Node(type) {}
    ~Expression() {}
};

class Statement : public Node
{
public:
    Statement() : Node() {}
    Statement(Type type) : Node(type) {}
    ~Statement() {}
};

class Comment : public Statement
{
public:
    Comment() : Statement(NODE_COMMENT) {}
    ~Comment() {}
    rapidjson::Value json(rapidjson::Document &father) override
    {
        rapidjson::Value nothing(rapidjson::kObjectType);
        return nothing;
    }
};