#pragma once
#include "node.h"
#include <memory>
#include <list>

class StatementBlock : public Statement
{
public:
    StatementBlock() : Statement(Type::NODE_STATEMENTBLOCK) {}
    ~StatementBlock() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        for (auto &stat : m_statements)
        {
            statements.PushBack(stat->json(father), father.GetAllocator());
        }
        json.AddMember("statements", statements, father.GetAllocator());
        return json;
    }

public:
    std::list<std::shared_ptr<Statement>> m_statements;
}
