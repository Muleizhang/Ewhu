#pragma once
#include "node.h"
#include "node.cpp"
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