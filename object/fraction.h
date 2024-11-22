#pragma once
#include "object.h"

class Ob_Fraction : public Object
{
public:
    Ob_Fraction() : Object(Object::OBJECT_FRACTION), m_numerator(0), m_denominator(1) {}
    Ob_Fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator) : Object(Object::OBJECT_FRACTION), m_numerator(numerator), m_denominator(denominator) {}
    ~Ob_Fraction() {}

    virtual std::string str() const
    {
        return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
    }

public:
    __INT64_TYPE__ m_numerator;
    __INT64_TYPE__ m_denominator;
};
