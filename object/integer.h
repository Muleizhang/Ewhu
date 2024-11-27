#pragma once
#include "object.h"

class Ob_Integer : public Object
{
public:
    Ob_Integer() : Object(Object::OBJECT_INTEGER), m_value(0) {}
    Ob_Integer(long long value) : Object(Object::OBJECT_INTEGER), m_value(value) {}
    ~Ob_Integer() {}

    virtual std::string str() const
    {
        return std::to_string(m_value);
    }

public:
    long long m_value;
};
