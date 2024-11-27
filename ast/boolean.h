#pragma once
#include "node.h"
class Boolean : public Expression
{
public:
    Boolean() : Expression(Type::NODE_BOOLEAN) {}
    ~Boolean() {};

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