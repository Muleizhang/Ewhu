#pragma once
#include "node.h"
#include <memory>
class IfStatement : public Statement
{
public:
    IfStatement() : Statement(Type::NODE_IFSTATEMENT) {}
    ~IfStatement() {}
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        json.AddMember("expression", m_expression->json(father), father.GetAllocator());
        json.AddMember("true_statement", m_true_statement->json(father), father.GetAllocator());
        // json.AddMember("false_statement", m_true_statement->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Expression> m_expression;
    std::shared_ptr<Statement> m_true_statement;
    // std::shared_ptr<Statement> m_false_statement;
};