#pragma once
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <string>
#include <memory>
#include <stdarg.h>
#include <stdexcept>

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
        OBJECT_BREAK,      // break
        //OBJECT_EMPTY,      // 空
    };

public:
    Object() {}
    Object(Type type) : m_type(type) {}
    Object(const Object &obj) : m_type(obj.m_type){};
    virtual ~Object(){};

    Type type() const { return m_type; }
    std::string name() const;
    virtual std::string str() const = 0;

    static std::shared_ptr<Object> new_error(const char *format, ...);

protected:
    Type m_type;
    static std::unordered_map<Type, std::string> m_names;
};

class Ob_Error : public Object
{
public:
    Ob_Error() : Object(Object::OBJECT_ERROR) {}
    Ob_Error(const std::string &message) : Object(Object::OBJECT_ERROR) {}
    ~Ob_Error() {}

    virtual std::string str() const
    {
        return m_messages;
    }

public:
    std::string m_messages;
};

class Ob_Identifier : public Object
{
public:
    Ob_Identifier() : Object(Object::OBJECT_IDENTIFIER), m_name(NULL), m_value(nullptr), m_type(Object::OBJECT_NULL) {}
    Ob_Identifier(std::string name) : Object(Object::OBJECT_IDENTIFIER), m_name(name), m_value(nullptr), m_type(Object::OBJECT_NULL) {}
    Ob_Identifier(std::string name, void *value, Object::Type type) : Object(Object::OBJECT_IDENTIFIER), m_name(name), m_value(value), m_type(type) {}
    ~Ob_Identifier() {}

    virtual std::string str() const
    {
        return m_name;
    }

public:
    std::string m_name;  // 变量名
    void *m_value;       // 指向变量的指针
    Object::Type m_type; // 变量类型
};

class Ob_Boolean : public Object
{
public:
    Ob_Boolean() : Object(Object::OBJECT_BOOLEAN), m_value(NULL) {}
    Ob_Boolean(bool value) : Object(Object::OBJECT_BOOLEAN), m_value(value) {}
    ~Ob_Boolean() {}

    virtual std::string str() const
    {
        return (m_value ? "true" : "false");
    }

public:
    bool m_value;
};

class Ob_Integer : public Object
{
public:
    Ob_Integer() : Object(Object::OBJECT_INTEGER), m_value(0) {}
    Ob_Integer(__INT64_TYPE__ value) : Object(Object::OBJECT_INTEGER), m_value(value) {}
    ~Ob_Integer() {}

    virtual std::string str() const
    {
        return std::to_string(m_value);
    }

public:
    __INT64_TYPE__ m_value;
};

class Ob_Fraction : public Object
{
public:
    Ob_Fraction() : Object(Object::OBJECT_FRACTION), m_integerPart(0), num(0), den(1) {}
    Ob_Fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator) : Object(Object::OBJECT_FRACTION), m_integerPart(0), num(numerator), den(denominator)
    {
        if (denominator == 0)
        {
            throw std::runtime_error("Denominator cannot be zero");
        }
        simplify();
    }
    ~Ob_Fraction() {}

    static Ob_Fraction simplify(const Ob_Fraction &fraction)
    {
        __INT64_TYPE__ gcd = std::gcd(fraction.num, fraction.den);
        return Ob_Fraction(fraction.num / gcd, fraction.den / gcd);
    }
    void simplify()
    {
        __INT64_TYPE__ gcd = std::gcd(num, den);
        num /= gcd;
        den /= gcd;
        if (den < 0)
        {
            num = -num;
            den = -den;
        }
    }

    virtual std::string realStr() const
    {
        return std::to_string(num) + "/" + std::to_string(den);
    }

    virtual std::string str() const
    {
        __INT64_TYPE__ integerPart = num / den;
        if (integerPart == 0)
        {
            return std::to_string(num) + "/" + std::to_string(den);
        }
        __INT64_TYPE__ decimalPart = num % den;
        if (decimalPart == 0)
        {
            return std::to_string(integerPart);
        }
        return std::to_string(integerPart) + "(" + std::to_string(decimalPart) + "/" + std::to_string(den) + ")";
    }

    static Ob_Fraction add(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->num * right->den + right->num * left->den,
                                    left->den * right->den));
    }
    static Ob_Fraction sub(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->num * right->den - right->num * left->den,
                                    left->den * right->den));
    }
    static Ob_Fraction mul(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->num * right->num, left->den * right->den));
    }
    static Ob_Fraction div(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->num * right->den, left->den * right->num));
    }
    static Ob_Fraction mod(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        __INT64_TYPE__ numerator = (left->num * right->den) % (left->den * right->num);
        __INT64_TYPE__ denominator = left->den * right->den;
        return simplify(Ob_Fraction(numerator, denominator));
    }
    Ob_Boolean equal(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den == right->num * den);
    }
    Ob_Boolean notEqual(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den != right->num * den);
    }
    Ob_Boolean lessThan(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den < right->num * den);
    }
    Ob_Boolean greaterThan(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den > right->num * den);
    }
    Ob_Boolean lessEqual(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den <= right->num * den);
    }
    Ob_Boolean greaterEqual(const std::shared_ptr<Ob_Fraction> &right) const
    {
        return Ob_Boolean(num * right->den >= right->num * den);
    }

    static Ob_Fraction decimalToFraction(__INT64_TYPE__ integerPart, __INT64_TYPE__ decimalPart)
    {

        // 计算小数部分的位数
        int decimalDigits = 0;
        int temp = decimalPart;
        while (temp > 0)
        {
            temp /= 10;
            decimalDigits++;
        }

        // 分子和分母的计算
        __INT64_TYPE__ denominator = std::pow(10, decimalDigits);
        __INT64_TYPE__ numerator = integerPart * denominator + decimalPart;

        // 约分
        __INT64_TYPE__ gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;

        return Ob_Fraction(numerator, denominator);
    }

public:
    __INT64_TYPE__ m_integerPart;
    __INT64_TYPE__ num;
    __INT64_TYPE__ den;
};

class Ob_String : public Object
{
public:
    Ob_String() : Object(Object::OBJECT_STRING), m_value(0) {}
    Ob_String(std::string value) : Object(Object::OBJECT_STRING), m_value(value) {}
    Ob_String(char value) : Object(Object::OBJECT_STRING), m_value(1, value) {}
    Ob_String(char *value) : Object(Object::OBJECT_STRING), m_value(value) {}
    ~Ob_String() {}

    virtual std::string str() const
    {
        return m_value;
    }

public:
    std::string m_value;
};

class Ob_Break : public Object
{
public:
    Ob_Break() : Object(Object::OBJECT_BREAK) {}
    ~Ob_Break() {}

    virtual std::string str() const
    {
        return "";
    }
};

class Ob_Null : public Object
{
public:
    Ob_Null() : Object(Object::OBJECT_NULL) {}
    ~Ob_Null() {}

    virtual std::string str() const
    {
        return "";
    }
};