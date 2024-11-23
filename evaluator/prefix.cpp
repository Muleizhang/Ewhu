#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_prefix(const TokenType &op, const std::shared_ptr<Object> &right)
{
    switch (right->type())
    {
    case Object::OBJECT_INTEGER:
    {
        return eval_integer_prefix_expression(op, right);
    }
    case Object::OBJECT_FRACTION:
    {
        return eval_fraction_prefix_expression(op, right);
    }
    default:
        break;
    }
    return new_error("Evaluator: unknown operator: %s %s", "operator", right->name().c_str());
}

std::shared_ptr<Object> Evaluator::eval_integer_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right)
{
    auto r = std::dynamic_pointer_cast<Ob_Integer>(right);
    if (op == TokenType::PLUS)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(r->m_value));
        return s;
    }
    else if (op == TokenType::MINUS)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(-r->m_value));
        return s;
    }
    else
    {
        return new_error("Evaluator: unknown operator: %s %s", "operator", right->name().c_str());
    }
}

std::shared_ptr<Object> Evaluator::eval_fraction_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right)
{
    auto r = std::dynamic_pointer_cast<Ob_Fraction>(right);
    if (op == TokenType::PLUS)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(r->m_numerator, r->m_denominator));
        return s;
    }
    else if (op == TokenType::MINUS)
    {
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(-r->m_numerator, r->m_denominator));
        return s;
    }
    else
    {
        return new_error("Evaluator: unknown operator: %s %s", "operator", right->name().c_str());
    }
}
