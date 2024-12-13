#pragma once
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <string>
#include <memory>
#include <stdarg.h>
#include <stdexcept>
#include <vector>
class Object
{
public:
    enum Type
    {
        OBJECT_ERROR = 0,
        OBJECT_TRIGNOMETRY, // 三角函数
        OBJECT_BOOLEAN,     // 布尔值
        OBJECT_INTEGER,     // 整数
        OBJECT_FRACTION,    // 分数
        OBJECT_STRING,      // 字符串
        OBJECT_IDENTIFIER,  // 标识符
        OBJECT_NULL,        // 用于空指针
        OBJECT_BREAK,       // break
        OBJECT_CONTINUE,    // continue
        OBJECT_RETURN,      // 函数返回
        OBJECT_ARRAY,       // 数组
        OBJECT_INDEX,
    };

public:
    Object() {}
    Object(Type type) : m_type(type) {}
    Object(const Object &obj) : m_type(obj.m_type) {};
    virtual ~Object() {};

    virtual std::shared_ptr<Object> clone() = 0;

    Type type() const { return m_type; }
    std::string name() const;
    virtual std::string str() const = 0;
    // virtual std::shared_ptr<Object> operator+() const = 0;

    static std::shared_ptr<Object> new_error(const char *format, ...);

public:
    Type m_type;
    static std::unordered_map<Type, std::string> m_names;

    // 字符串
    std::string m_string;
    // 错误信息
    std::string m_messages;
    // 变量名
    std::string m_name;
    // 指向变量的指针
    void *m_value;
    // 变量类型
    Object::Type m_value_type;
    // 整数 or 布尔值
    long long m_int;
    // 分数整数部分
    long long m_integerPart;
    // 分数分子
    long long num;
    // 分数分母
    long long den;
    std::vector<std::shared_ptr<Object>> m_array;
};

class Ob_Error : public Object
{
public:
    Ob_Error() : Object(Object::OBJECT_ERROR) {}
    Ob_Error(const std::string &message) : Object(Object::OBJECT_ERROR) {}
    Ob_Error(const Ob_Error &obj) : Object(Object::OBJECT_ERROR) { m_messages = obj.m_messages; }
    ~Ob_Error() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Error>(*this);
    }
    virtual std::string str() const
    {
        return m_messages;
    }

public:
};

class Ob_Identifier : public Object
{
public:
    Ob_Identifier() : Object(Object::OBJECT_IDENTIFIER) {}
    Ob_Identifier(std::string name) : Object(Object::OBJECT_IDENTIFIER) { m_name = name; }
    Ob_Identifier(std::string name, void *value, Object::Type type) : Object(Object::OBJECT_IDENTIFIER)
    {
        m_name = name, m_value = value, m_value_type = type;
    }
    Ob_Identifier(const Ob_Identifier &obj) : Object(Object::OBJECT_IDENTIFIER)
    {
        m_name = obj.m_name;
        m_value = obj.m_value;
        m_value_type = obj.m_value_type;
    }
    ~Ob_Identifier() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Identifier>(*this);
    }
    virtual std::string str() const
    {
        return m_name;
    }

public:
};

class Ob_Boolean : public Object
{
public:
    Ob_Boolean() : Object(Object::OBJECT_BOOLEAN) {}
    Ob_Boolean(long long value) : Object(Object::OBJECT_BOOLEAN) { m_int = value; }
    Ob_Boolean(const Ob_Boolean &obj) : Object(Object::OBJECT_BOOLEAN) { m_int = obj.m_int; }
    ~Ob_Boolean() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Boolean>(*this);
    }
    virtual std::string str() const
    {
        return (m_int ? "true" : "false");
    }

public:
};

class Ob_Integer : public Object
{
public:
    Ob_Integer() : Object(Object::OBJECT_INTEGER) { m_int = (0); }
    Ob_Integer(long long value) : Object(Object::OBJECT_INTEGER) { m_int = (value); }
    Ob_Integer(const Ob_Integer &obj) : Object(Object::OBJECT_INTEGER) { m_int = obj.m_int; }
    ~Ob_Integer() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Integer>(*this);
    }
    virtual std::string str() const
    {
        return std::to_string(m_int);
    }
};

class Ob_Fraction : public Object
{
public:
    Ob_Fraction() : Object(Object::OBJECT_FRACTION)
    {
        m_integerPart = (0);
        num = (0);
        den = (1);
    }
    Ob_Fraction(long long numerator, long long denominator) : Object(Object::OBJECT_FRACTION)
    {
        m_integerPart = (0);
        num = (numerator);
        den = (denominator);

        simplify();
    }
    Ob_Fraction(const Ob_Fraction &fraction) : Object(Object::OBJECT_FRACTION)
    {
        m_integerPart = (fraction.m_integerPart);
        num = (fraction.num);
        den = (fraction.den);
    }
    ~Ob_Fraction() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Fraction>(*this);
    }

    static Ob_Fraction simplify(const Ob_Fraction &fraction)
    {
        long long gcd = std::gcd(fraction.num, fraction.den);
        return Ob_Fraction(fraction.num / gcd, fraction.den / gcd);
    }
    void simplify()
    {
        long long gcd = std::gcd(num, den);
        num /= gcd;
        den /= gcd;
        if (den < 0)
        {
            num = -num;
            den = -den;
        }
    }

    //***********************************************************写三角函数！！！！！！！！！！！！！！！！！！！ */

    virtual std::string realStr() const
    {
        return std::to_string(num) + "/" + std::to_string(den);
    }

    virtual std::string str() const
    {
        long long integerPart = num / den;
        if (integerPart == 0)
        {
            return std::to_string(num) + "/" + std::to_string(den);
        }
        long long decimalPart = num % den;
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
        long long numerator = (left->num * right->den) % (left->den * right->num);
        long long denominator = left->den * right->den;
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

    static Ob_Fraction decimalToFraction(long long integerPart, long long decimalPart)
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
        long long denominator = std::pow(10, decimalDigits);
        long long numerator = integerPart * denominator + decimalPart;

        // 约分
        long long gcd = std::gcd(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;

        return Ob_Fraction(numerator, denominator);
    }

public:
};
/*
class Ob_Trignometry : public Object
{
public:
    Ob_Trignometry() : Object(Object::OBJECT_TRIGNOMETRY) {}
    Ob_Trignometry(std::string name) : Object(Object::OBJECT_TRIGNOMETRY) { m_name = name; }
    Ob_Trignometry(std::string name, void *value, Object::Type type) : Object(Object::OBJECT_TRIGNOMETRY)
    {
        m_name = name;
        m_value = value;
        m_type = type;
    }

    double sinValue() const
    {
        return std::sin(*static_cast<double *>(m_value));
    }

    double cosValue() const
    {
        return std::cos(*static_cast<double *>(m_value));
    }

    double tanValue() const
    {
        return std::tan(*static_cast<double *>(m_value));
    }

    std::string sinStr() const
    {
        return "sin(" + std::to_string(*static_cast<double *>(m_value)) + ")";
    }

    std::string cosStr() const
    {
        return "cos(" + std::to_string(*static_cast<double *>(m_value)) + ")";
    }

    std::string tanStr() const
    {
        return "tan(" + std::to_string(*static_cast<double *>(m_value)) + ")";
    }

private:
    std::string m_name;
    void *m_value;
    Object::Type m_type;
};
*/

class Ob_String : public Object
{
public:
    Ob_String() : Object(Object::OBJECT_STRING) {}
    Ob_String(std::string value) : Object(Object::OBJECT_STRING) { m_string = (value); }
    Ob_String(char value) : Object(Object::OBJECT_STRING) { m_string = (1, value); }
    Ob_String(char *value) : Object(Object::OBJECT_STRING) { m_string = (value); }
    Ob_String(const Ob_String &obj) : Object(Object::OBJECT_STRING) { m_string = obj.m_string; }
    ~Ob_String() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_String>(*this);
    }

    virtual std::string str() const
    {
        return "'" + m_string + "'";
    }

public:
};

class Ob_Break : public Object
{
public:
    Ob_Break() : Object(Object::OBJECT_BREAK) {}
    ~Ob_Break() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Break>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }
};

class Ob_Continue : public Object
{
public:
    Ob_Continue() : Object(Object::OBJECT_CONTINUE) {}
    ~Ob_Continue() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Continue>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }
};

class Ob_Return : public Object
{
public:
    Ob_Return() : Object(Object::OBJECT_RETURN) {}
    ~Ob_Return() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Return>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }

public:
    std::shared_ptr<Object> m_expression;
};

class Ob_Null : public Object
{
public:
    Ob_Null() : Object(Object::OBJECT_NULL) {}
    ~Ob_Null() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Null>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }
};

class Ob_Array : public Object, public std::enable_shared_from_this<Ob_Array>
{
public:
    Ob_Array() : Object(Object::OBJECT_ARRAY) {}
    Ob_Array(const Ob_Array &obj) : Object(Object::OBJECT_ARRAY) { m_array = obj.m_array; }
    ~Ob_Array() {}

    virtual std::shared_ptr<Object> clone() override
    {
        return std::dynamic_pointer_cast<Object>(shared_from_this());
    }

    std::shared_ptr<Object> add(const std::shared_ptr<Object> &obj)
    {
        m_array.insert(m_array.end(), obj->m_array.begin(), obj->m_array.end());
        return std::make_shared<Ob_Array>(*this);
    }
    virtual std::string str() const
    {
        if (!m_array.empty())
        {
            std::string r;
            r += '[';
            for (auto &i : Object::m_array)
            {
                r += i->str();
                r += ',';
            }
            r.pop_back();
            r += ']';
            return r;
        }
        else
        {
            return "[]";
        }
    }

public:
};

class Ob_Index : public Object
{
public:
    Ob_Index() : Object(Object::OBJECT_INDEX) {}
    ~Ob_Index() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Index>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }

public:
    std::shared_ptr<Object> m_array;
    std::shared_ptr<Ob_Integer> m_index;
};