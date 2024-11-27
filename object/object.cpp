#include "object.h"
#include "boolean.h"
#include "integer.h"
#include "string.h"
#include "fraction.h"
#include "identifier.h"
#include "error.h"
#include <string>
#include <stdarg.h>
std::map<Object::Type, std::string> Object::m_names = {
    {Object::OBJECT_ERROR, "Error"},
    {Object::OBJECT_BOOLEAN, "Boolean"},
    {Object::OBJECT_INTEGER, "Integer"},
    {Object::OBJECT_FRACTION, "Fraction"},
    {Object::OBJECT_STRING, "String"},
    {Object::OBJECT_IDENTIFIER, "Identifier"},
    {Object::OBJECT_NULL, "Null"},
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

std::shared_ptr<Object> Object::new_boolean(bool value)
{
    std::shared_ptr<Ob_Boolean> e(new Ob_Boolean(value));
    return e;
}

std::shared_ptr<Object> Object::new_integer(long long value)
{
    std::shared_ptr<Ob_Integer> e(new Ob_Integer(value));
    return e;
}

std::shared_ptr<Object> Object::new_fraction(long long numerator, long long denominator)
{
    std::shared_ptr<Ob_Fraction> e(new Ob_Fraction(numerator, denominator));
    return e;
}

std::shared_ptr<Object> Object::new_string(const std::string &value)
{
    std::shared_ptr<Ob_String> e(new Ob_String(value));
    return e;
}

std::shared_ptr<Object> Object::new_identifier(const std::string &value)
{
    std::shared_ptr<Ob_Identifier> e(new Ob_Identifier(value));
    return e;
}