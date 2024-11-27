#pragma once
#include "object.h"
#include "boolean.h"
#include <cmath>
#include <numeric>
#ifdef __linux__
#include <stdexcept>
#endif

class Ob_Fraction : public Object
{
public:
    Ob_Fraction() : Object(Object::OBJECT_FRACTION), m_integerPart(0), num(0), den(1) {}
    Ob_Fraction(long long numerator, long long denominator) : Object(Object::OBJECT_FRACTION), m_integerPart(0), num(numerator), den(denominator)
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
    long long m_integerPart;
    long long num;
    long long den;
};
