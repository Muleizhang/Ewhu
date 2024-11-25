#pragma once
#include "object.h"
#include <cmath>
#include <numeric>

class Ob_Fraction : public Object
{
public:
    Ob_Fraction() : Object(Object::OBJECT_FRACTION), m_integerPart(0), m_numerator(0), m_denominator(1) {}
    Ob_Fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator) : Object(Object::OBJECT_FRACTION), m_integerPart(0), m_numerator(numerator), m_denominator(denominator)
    {
        simplify();
    }
    ~Ob_Fraction() {}

    static Ob_Fraction simplify(const Ob_Fraction &fraction)
    {
        __INT64_TYPE__ gcd = std::gcd(fraction.m_numerator, fraction.m_denominator);
        return Ob_Fraction(fraction.m_numerator / gcd, fraction.m_denominator / gcd);
    }
    void simplify()
    {
        __INT64_TYPE__ gcd = std::gcd(m_numerator, m_denominator);
        m_numerator /= gcd;
        m_denominator /= gcd;
    }

    virtual std::string realStr() const
    {
        return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
    }

    virtual std::string str() const
    {
        __INT64_TYPE__ integerPart = m_numerator / m_denominator;
        if (integerPart == 0)
        {
            return std::to_string(m_numerator) + "/" + std::to_string(m_denominator);
        }
        __INT64_TYPE__ decimalPart = m_numerator % m_denominator;
        if (decimalPart == 0)
        {
            return std::to_string(integerPart);
        }
        return std::to_string(integerPart) + "(" + std::to_string(decimalPart) + "/" + std::to_string(m_denominator) + ")";
    }

    static Ob_Fraction add(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->m_numerator * right->m_denominator + right->m_numerator * left->m_denominator,
                                    left->m_denominator * right->m_denominator));
    }
    static Ob_Fraction sub(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->m_numerator * right->m_denominator - right->m_numerator * left->m_denominator,
                                    left->m_denominator * right->m_denominator));
    }
    static Ob_Fraction mul(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->m_numerator * right->m_numerator, left->m_denominator * right->m_denominator));
    }
    static Ob_Fraction div(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        return simplify(Ob_Fraction(left->m_numerator * right->m_denominator, left->m_denominator * right->m_numerator));
    }
    static Ob_Fraction mod(const std::shared_ptr<Ob_Fraction> &left, const std::shared_ptr<Ob_Fraction> &right)
    {
        __INT64_TYPE__ numerator = (left->m_numerator * right->m_denominator) % (left->m_denominator * right->m_numerator);
        __INT64_TYPE__ denominator = left->m_denominator * right->m_denominator;
        return simplify(Ob_Fraction(numerator, denominator));
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
    __INT64_TYPE__ m_numerator;
    __INT64_TYPE__ m_denominator;
};
