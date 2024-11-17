#pragma once
#include "node.h"
#include <memory>
class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() : Statement(Type::NODE_EXPRESSION_STATEMENT) {}
    ~ExpressionStatement() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("expression", m_expression->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Expression> m_expression;
};