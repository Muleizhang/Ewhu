#pragma once
#include "node.h"
class Integer : public Expression
{
public:
    Integer() : Expression(Type::NODE_INTEGER) {}
    ~Integer() {};

public:
    __INT64_TYPE__ m_value;
};