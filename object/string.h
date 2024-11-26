#pragma once
#include "object.h"

class Ob_String : public Object
{
public:
    Ob_String() : Object(Object::OBJECT_STRING), m_value(0) {}
    Ob_String(std::string value) : Object(Object::OBJECT_STRING), m_value(value) {}
    ~Ob_String() {}

    virtual std::string str() const
    {
        return m_value;
    }

public:
    std::string m_value;
};
