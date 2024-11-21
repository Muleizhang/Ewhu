#pragma once
#include "object.h"
class Ob_Error : public Object
{
public:
    Ob_Error() : Object(Object::OBJECT_ERROR) {}
    Ob_Error(const std::string &message) : Object(Object::OBJECT_ERROR) {}
    ~Ob_Error() {}

    virtual std::string str() const
    {
        return "error: " + m_messages;
    }

public:
    std::string m_messages;
};

