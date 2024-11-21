#pragma once
#include "object.h"
#include "integer.h"
#include "error.h"
#include <string>
#include <stdarg.h>
std::map<Object::Type, std::string> Object::m_names = {
    {Object::OBJECT_ERROR, "error"},
    {Object::OBJECT_INTEGER, "integer"},
};

std::string Object::name() const
{
    auto it = m_names.find(m_type);
    if (it != m_names.end())
    {
        return it->second;
    }
    return "";
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

std::shared_ptr<Object> Object::new_integer(__INT64_TYPE__ value)
{
    std::shared_ptr<Ob_Integer> e(new Ob_Integer(value));
    return e;
}
