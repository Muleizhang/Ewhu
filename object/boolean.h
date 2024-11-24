#pragma once
#include "object.h"

class Ob_Boolean : public Object
{
public:
    Ob_Boolean() : Object(Object::OBJECT_BOOLEAN), m_value(NULL) {}
    Ob_Boolean(bool value) : Object(Object::OBJECT_BOOLEAN), m_value(value) {}
    ~Ob_Boolean() {}

    virtual std::string str() const
    {
        return (m_value ? "true" : "false");
    }

public:
    bool m_value;
};
