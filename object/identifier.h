#pragma once
#include "object.h"

class Ob_Identifier : public Object
{
public:
    Ob_Identifier() : Object(Object::OBJECT_IDENTIFIER), m_name(NULL), m_value(nullptr), m_type(Object::OBJECT_NULL) {}
    Ob_Identifier(std::string name) : Object(Object::OBJECT_IDENTIFIER), m_name(name), m_value(nullptr), m_type(Object::OBJECT_NULL) {}
    Ob_Identifier(std::string name, void *value, Object::Type type) : Object(Object::OBJECT_IDENTIFIER), m_name(name), m_value(value), m_type(type) {}
    ~Ob_Identifier() {}

    virtual std::string str() const
    {
        return m_name;
    }

public:
    std::string m_name;  // 变量名
    void *m_value;       // 指向变量的指针
    Object::Type m_type; // 变量类型
};
