#pragma once
#include <map>
#include <string>
#include <memory>
#include <stdarg.h>
class Object
{
public:
    enum Type
    {
        OBJECT_ERROR = 0,
        OBJECT_INTEGER,
    };

public:
    Object() {}
    Object(Type type) : m_type(type) {}
    virtual ~Object(){};

    Type type() const { return m_type; }
    std::string name() const;
    virtual std::string str() const = 0;

    static std::shared_ptr<Object> new_error(const char *format, ...);
    static std::shared_ptr<Object> new_integer(__INT64_TYPE__ value);

protected:
    Type m_type;
    static std::map<Type, std::string> m_names;
};
