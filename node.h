#pragma once
#include "Token.h"
#include <map>
enum Type
{
    NODE_INTEGER = 0,
    NODE_INFIX,
    NODE_EXPRESSION_STATEMENT,
    NODE_PROGRAM
};

class Node // 一个节点的类
{
public:
    Type m_type;
    Token m_token;
    static std::map<Type, std::string> m_names;

public:
    Node() {} // 构造函数
    Node(Type type) : m_type(type) {}
    virtual ~Node() {}

    Type type() const { return m_type; }
    std::string name() const;
};

class Expression : public Node // 表达式
{
public:
    Expression() : Node() {}
    Expression(Type type) : Node(type) {}
    ~Expression() {}
};

class Statement : public Node // 语句
{
public:
    Statement() : Node() {}
    Statement(Type type) : Node(type) {}
    ~Statement() {}
};