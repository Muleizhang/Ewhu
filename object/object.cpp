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

std::shared_ptr<Object> new_error(const char *format, ...)
{
    char buf[114514] = {0};
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(buf, sizeof(buf), format, arg_ptr);
    va_end(arg_ptr);

    std::shared_ptr<Ob_Error> obj(new Ob_Error());
    obj->m_messages = buf;
    return obj;
}
