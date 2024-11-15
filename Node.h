#pragma once

#include <memory>
#include "Token.h"

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
    Node(Type type) : m_type(type) {};
    virtual ~Node() {}

    Type getType() { return m_type; }
    std::string nameString();

public:
    Type m_type;
    Token m_token;
    static std::map<Type, std::string> m_names;
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
