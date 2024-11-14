#pragma once

#include <memory>
#include "token.h"

class Node
{
public:
    enum Type
    {
        NODE_INTEGER = 0,
        NODE_FLOAT,
        NODE_STRING,
        NODE_BINARY,
        NODE_INFIX,
        NODE_IDENTIFIER,
        NODE_EXPRESSION_STATEMENT,
        NODE_PROGRAM,
    };

    Node() {}
    Node(Type type) : type(type) {};
    virtual ~Node() {}

    Type getType() { return type; }
    std::string nameString();

public:
    Type type;
    Token token;
    static std::map<Type, std::string> TypeToString;
};

class Statement : public Node
{
public:
    Statement(Type type) : Node(type) {}
    ~Statement() {}
};

class Expression : public Node
{
public:
    Expression(Type type) : Node(type) {}
    ~Expression() {}
};

class Integer : public Expression
{
public:
    int64_t value;

public:
    Integer() : Expression(NODE_INTEGER) {}
    ~Integer() {}
};

class Infix : public Expression
{
public:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
    Token op;

public:
    Infix() : Expression(NODE_INFIX) {}
    ~Infix() {}
};

class Program : public Statement
{
public:
    std::vector<std::shared_ptr<Statement>> statements;

public:
    Program() : Statement(NODE_PROGRAM) {}
    ~Program() {}
};

class ExpressionStatement : public Statement
{
public:
    std::shared_ptr<Expression> expression;

public:
    ExpressionStatement() : Statement(NODE_EXPRESSION_STATEMENT) {}
    ~ExpressionStatement() {}
};
