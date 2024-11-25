#pragma once
#include "object.h"
#include "boolean.h"
#include <cmath>
#include <numeric>

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
