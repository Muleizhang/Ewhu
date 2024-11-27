#pragma once
#include <memory>
#include <unordered_map>
#include "../lexer/token.h"
#include "../rapidjson/include/rapidjson/document.h"
#include "../rapidjson/include/rapidjson/writer.h"
#include "../rapidjson/include/rapidjson/stringbuffer.h"
#include <iostream>

enum class Type
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

};

class Node
{
public:
    Node() {}
    Node(Type type) : m_type(type) {}
    virtual ~Node() {}

    Type type() { return m_type; }
    std::string name() const
    {
        auto it = m_names.find(m_type);
        if (it == m_names.end())
        {
            return "";
        }
        return it->second;
    };
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
    Comment() : Statement(Type::NODE_COMMENT) {}
    ~Comment() {}
    rapidjson::Value json(rapidjson::Document &father) override
    {
        rapidjson::Value nothing(rapidjson::kObjectType);
        return nothing;
    }
};

class String : public Expression
{
public:
    String() : Expression(Type::NODE_STRING) {}
    ~String() {};

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        std::string *valueStr = new std::string;
        *valueStr = m_value;
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("value", rapidjson::StringRef(valueStr->c_str()), father.GetAllocator());
        return json;
    }

public:
    std::string m_value;
};