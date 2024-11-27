#pragma once
#include "node.h"

class Integer : public Expression
{
public:
    Integer() : Expression(Type::NODE_INTEGER) {}
    ~Integer() {};

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
    long long m_value;
};