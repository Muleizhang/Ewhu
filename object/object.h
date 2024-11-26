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
        OBJECT_BOOLEAN,    // 布尔值
        OBJECT_INTEGER,    // 整数
        OBJECT_FRACTION,   // 分数
        OBJECT_STRING,     // 字符串
        OBJECT_IDENTIFIER, // 标识符
        OBJECT_NULL,       // 用于空指针
    };

public:
    Object() {}
    Object(Type type) : m_type(type) {}
    Object(const Object &obj) : m_type(obj.m_type) {};
    virtual ~Object() {};

    Type type() const { return m_type; }
    std::string name() const;
    virtual std::string str() const = 0;

    static std::shared_ptr<Object> new_error(const char *format, ...);
    static std::shared_ptr<Object> new_boolean(bool value);
    static std::shared_ptr<Object> new_integer(__INT64_TYPE__ value);
    static std::shared_ptr<Object> new_fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator);
    static std::shared_ptr<Object> new_string(const std::string &value);
    static std::shared_ptr<Object> new_identifier(const std::string &value);

protected:
    Type m_type;
    static std::map<Type, std::string> m_names;
};
