#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_infix(const TokenType op, const std::shared_ptr<Object> &left,
                                              const std::shared_ptr<Object> &right) // 中缀表达式求值
{
    switch (left->type())
    {
    case Object::OBJECT_INTEGER:
        if (right->type() == Object::OBJECT_INTEGER)
            return eval_integer_infix_expression(op, left, right); // 整数与整数的运算
        break;
    case Object::OBJECT_FRACTION:
        if (right->type() == Object::OBJECT_FRACTION)
            return eval_fraction_infix_expression(op, left, right); // 分数与分数的运算
        break;
    default:
        break;
    }
    return new_error("unknown operator: %s %s %s", left->name().c_str(), "operator", right->name().c_str());
}

std::shared_ptr<Object> Evaluator::eval_integer_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                                 const std::shared_ptr<Object> &right)
{
    auto l = std::dynamic_pointer_cast<Ob_Integer>(left);
    auto r = std::dynamic_pointer_cast<Ob_Integer>(right);
    if (op == TokenType::PLUS)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value + r->m_value));
        return s;
    }
    else if (op == TokenType::MINUS)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value - r->m_value));
        return s;
    }
    else if (op == TokenType::STAR)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value * r->m_value));
        return s;
    }
    else if (op == TokenType::SLASH)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value / r->m_value));
        return s;
    }
    else if (op == TokenType::PERCENT)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value % r->m_value));
        return s;
    }
    else if (op == TokenType::DOT) // 分数
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_value, r->m_value));
        return s;
    }
    else
    {
        return new_error("unknown operator: %s %s %s", left->name().c_str(), "operator", right->name().c_str());
    }
}
std::shared_ptr<Object> Evaluator::eval_fraction_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                                  const std::shared_ptr<Object> &right)
{
    auto l = std::dynamic_pointer_cast<Ob_Fraction>(left);
    auto r = std::dynamic_pointer_cast<Ob_Fraction>(right);
    if (op == TokenType::PLUS)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_numerator * r->m_denominator + r->m_numerator * l->m_denominator,
                                                       l->m_denominator * r->m_denominator));
        return s;
    }
    else if (op == TokenType::MINUS)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_numerator * r->m_denominator - r->m_numerator * l->m_denominator,
                                                       l->m_denominator * r->m_denominator));
        return s;
    }
    else if (op == TokenType::STAR)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_numerator * r->m_numerator, l->m_denominator * r->m_denominator));
        return s;
    }
    else if (op == TokenType::SLASH)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_numerator * r->m_denominator, l->m_denominator * r->m_numerator));
        return s;
    }
    else if (op == TokenType::PERCENT)
    {
        // 分数取余操作
        int numerator = (l->m_numerator * r->m_denominator) % (l->m_denominator * r->m_numerator);
        int denominator = l->m_denominator * r->m_denominator;
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(numerator, denominator));
        return s;
    }
    else
    {
        return new_error("unknown operator: %s %s %s", left->name().c_str(), "operator", right->name().c_str());
    }
}