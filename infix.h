#pragma once
#include <memory> //智能指针
#include "node.h"

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