#pragma once
#include <memory> //智能指针
#include "node.h"

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