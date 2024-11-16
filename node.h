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
    Node(Type type) : m_type(type) {}
    virtual ~Node() {}

    Type type() { return m_type; }
    std::string name() const;

public:
    Type m_type;
    Token m_token;
    static std::map<Type, std::string> m_names;
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
