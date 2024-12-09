#pragma once
#include "node.h"
#include <vector>
class Identifier : public Expression
{
public:
    Identifier() : Expression(Type::NODE_IDENTIFIER) {}
    ~Identifier(){};

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        std::string *valueStr = new std::string;
        *valueStr = m_name;
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("value", rapidjson::StringRef(valueStr->c_str()), father.GetAllocator());
        return json;
    }

public:
    std::string m_name;
};

class Integer : public Expression
{
public:
    Integer() : Expression(Type::NODE_INTEGER) {}
    ~Integer(){};

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        std::string *valueStr = new std::string;
        *valueStr = std::to_string(m_value);
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("value", rapidjson::StringRef(valueStr->c_str()), father.GetAllocator());
        return json;
    }

public:
    __INT64_TYPE__ m_value;
};

class Boolean : public Expression
{
public:
    Boolean() : Expression(Type::NODE_BOOLEAN) {}
    ~Boolean(){};

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        std::string *valueStr = new std::string;
        *valueStr = m_value ? "true" : "false";
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("value", rapidjson::StringRef(valueStr->c_str()), father.GetAllocator());
        return json;
    }

public:
    bool m_value;
};

class String : public Expression
{
public:
    String() : Expression(Type::NODE_STRING) {}
    ~String(){};

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

class Infix : public Expression // 中缀表达式
{
public:
    Infix() : Expression(Type::NODE_INFIX) {}
    ~Infix() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        if (TokenTypeToString.find(m_operator) == TokenTypeToString.end())
        {
            json.AddMember("operator", "err", father.GetAllocator());
        }
        else
        {
            json.AddMember("operator", rapidjson::StringRef(TokenTypeToString[m_operator].c_str()), father.GetAllocator());
        }

        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("left", m_left->json(father), father.GetAllocator());
        json.AddMember("right", m_right->json(father), father.GetAllocator());
        return json;
    }

public:
    TokenType m_operator;                // 运算符
    std::shared_ptr<Expression> m_left;  // 左表达式
    std::shared_ptr<Expression> m_right; // 右表达式
};

class Prefix : public Expression // 前缀表达式
{
public:
    Prefix() : Expression(Type::NODE_PREFIX) {}
    ~Prefix() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        switch (m_operator)
        {
        case TokenType::PLUS:
            json.AddMember("operator", "+", father.GetAllocator());
            break;
        case TokenType::MINUS:
            json.AddMember("operator", "-", father.GetAllocator());
            break;
        case TokenType::STAR:
            json.AddMember("operator", "*", father.GetAllocator());
            break;
        case TokenType::SLASH:
            json.AddMember("operator", "/", father.GetAllocator());
            break;
        default:
            json.AddMember("operator", "err", father.GetAllocator());
            break;
        }

        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("right", m_right->json(father), father.GetAllocator());
        return json;
    }

public:
    TokenType m_operator;                // 运算符
    std::shared_ptr<Expression> m_right; // 右表达式
};

class FunctionIdentifier : public Expression // 函数的调用
{
public:
    FunctionIdentifier() : Expression(Type::NODE_FUNCTION_IDENTIFIER) {}
    ~FunctionIdentifier(){};

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        std::string *valueStr = new std::string;
        *valueStr = m_name;
        rapidjson::Value args(rapidjson::kArrayType);
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        for (auto &arg : m_initial_list)
        {
            args.PushBack(arg->json(father), father.GetAllocator());
        }
        json.AddMember("arguments", args, father.GetAllocator());
        json.AddMember("value", rapidjson::StringRef(valueStr->c_str()), father.GetAllocator());
        return json;
    }

public:
    std::string m_name;
    std::vector<std::shared_ptr<Expression>> m_initial_list;
};

class Array : public Expression // 数组
{
public:
    Array() : Expression(Type::NODE_ARRAY){};
    ~Array(){};
    
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());

        rapidjson::Value args(rapidjson::kArrayType);
        for (auto &arg : m_array)
        {
            args.PushBack(arg->json(father), father.GetAllocator());
        }
        json.AddMember("array", args, father.GetAllocator());
       
        return json;
    }
public:
    std::vector<std::shared_ptr<Expression>> m_array;
};