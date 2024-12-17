#include "object.h"
#include <string>
// #include <stdarg.h>

std::unordered_map<Object::Type, std::string> Object::m_names = {
    {Object::OBJECT_ERROR, "Error"},
    {Object::OBJECT_BOOLEAN, "Boolean"},
    {Object::OBJECT_INTEGER, "Integer"},
    {Object::OBJECT_FRACTION, "Fraction"},
    {Object::OBJECT_STRING, "String"},
    {Object::OBJECT_IDENTIFIER, "Identifier"},
    {Object::OBJECT_NULL, "Null"},
    {Object::OBJECT_BREAK, "Break"},
    {Object::OBJECT_RETURN, "Return"},
    {Object::OBJECT_ARRAY, "Array"},
};

std::string Object::name() const
{
    auto it = m_names.find(m_type);
    if (it != m_names.end())
    {
        return it->second;
    }
    return "UnknownType";
}
