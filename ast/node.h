#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include "../lexer/token.h"
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"

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