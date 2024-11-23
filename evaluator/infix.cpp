#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_infix(const TokenType op, const std::shared_ptr<Object> &left,
                                              const std::shared_ptr<Object> &right) // 中缀表达式求值
{
    // 整数与整数的运算
    if (left->type() == Object::OBJECT_INTEGER && right->type() == Object::OBJECT_INTEGER)
        return eval_integer_infix_expression(op, left, right);
    // 分数的运算
    if (left->type() == Object::OBJECT_FRACTION && right->type() == Object::OBJECT_FRACTION)
        return eval_fraction_infix_expression(op, left, right);
    // 分数与整数的运算
    if (left->type() == Object::OBJECT_FRACTION && right->type() == Object::OBJECT_INTEGER)
        return eval_fraction_infix_expression(op, left, std::make_shared<Ob_Fraction>(std::dynamic_pointer_cast<Ob_Integer>(right)->m_value, 1));
    // 整数与分数的运算
    if (left->type() == Object::OBJECT_INTEGER && right->type() == Object::OBJECT_FRACTION)
        return eval_fraction_infix_expression(op, std::make_shared<Ob_Fraction>(std::dynamic_pointer_cast<Ob_Integer>(left)->m_value, 1), right);

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
        std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_value, r->m_value));
        return s;
    }
    else if (op == TokenType::SLASH_SLASH)
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
        // 小数用分数表达
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::decimalToFraction(l->m_value, r->m_value));
        return s;
    }
    else if (op == TokenType::EQUAL_EQUAL)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value == r->m_value));
        return s;
    }
    else if (op == TokenType::BANG_EQUAL)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value != r->m_value));
        return s;
    }
    else if (op == TokenType::LESS)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value < r->m_value));
        return s;
    }
    else if (op == TokenType::GREATER)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value > r->m_value));
        return s;
    }
    else if (op == TokenType::LESS_EQUAL)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value <= r->m_value));
        return s;
    }
    else if (op == TokenType::GREATER_EQUAL)
    {
        std::shared_ptr<Ob_Integer> s(new Ob_Integer(l->m_value >= r->m_value));
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
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::add(l, r));
        return s;
    }
    else if (op == TokenType::MINUS)
    {
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::sub(l, r));
        return s;
    }
    else if (op == TokenType::STAR)
    {
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::mul(l, r));
        return s;
    }
    else if (op == TokenType::SLASH)
    {
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::div(l, r));
        return s;
    }
    else if (op == TokenType::PERCENT)
    {
        std::shared_ptr<Ob_Fraction> s =
            std::make_shared<Ob_Fraction>(Ob_Fraction::mod(l, r));
        return s;
    }
    else
    {
        return new_error("unknown operator: %s %s %s", left->name().c_str(), "operator", right->name().c_str());
    }
}