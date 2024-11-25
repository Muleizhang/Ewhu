#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_infix(const TokenType op, const std::shared_ptr<Object> &left,
                                              const std::shared_ptr<Object> &right) // 中缀表达式求值
{
    // assign
    if (op == TokenType::EQUAL)
    {
        return eval_assign_expression(left, right);
    }
    // int op int
    if (left->type() == Object::OBJECT_INTEGER && right->type() == Object::OBJECT_INTEGER)
        return eval_integer_infix_expression(op, left, right);
    // fraction op fraction
    if (left->type() == Object::OBJECT_FRACTION && right->type() == Object::OBJECT_FRACTION)
        return eval_fraction_infix_expression(op, left, right);
    // int op fraction
    if (left->type() == Object::OBJECT_FRACTION && right->type() == Object::OBJECT_INTEGER)
        return eval_fraction_infix_expression(op, left, std::make_shared<Ob_Fraction>(std::dynamic_pointer_cast<Ob_Integer>(right)->m_value, 1));
    // fraction op int
    if (left->type() == Object::OBJECT_INTEGER && right->type() == Object::OBJECT_FRACTION)
        return eval_fraction_infix_expression(op, std::make_shared<Ob_Fraction>(std::dynamic_pointer_cast<Ob_Integer>(left)->m_value, 1), right);

    return new_error("Evaluator::eval_infix unknown operation: %s %s %s", left->name().c_str(), TokenTypeToString[op].c_str(), right->name().c_str());
}

std::shared_ptr<Object> Evaluator::eval_integer_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                                 const std::shared_ptr<Object> &right)
{
    auto l = std::dynamic_pointer_cast<Ob_Integer>(left);
    auto r = std::dynamic_pointer_cast<Ob_Integer>(right);
    if (op == TokenType::PLUS)
    {
        return std::make_shared<Ob_Integer>(l->m_value + r->m_value);
    }
    if (op == TokenType::MINUS)
    {
        return std::make_shared<Ob_Integer>(l->m_value - r->m_value);
    }
    if (op == TokenType::STAR)
    {
        return std::make_shared<Ob_Integer>(l->m_value * r->m_value);
    }
    if (op == TokenType::SLASH)
    {
        return std::make_shared<Ob_Fraction>(l->m_value, r->m_value);
    }
    if (op == TokenType::SLASH_SLASH)
    {
        return std::make_shared<Ob_Integer>(l->m_value / r->m_value);
    }
    if (op == TokenType::PERCENT)
    {
        return std::make_shared<Ob_Integer>(l->m_value % r->m_value);
    }
    if (op == TokenType::DOT) // 分数
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::decimalToFraction(l->m_value, r->m_value));
    }
    if (op == TokenType::EQUAL_EQUAL)
    {
        return std::make_shared<Ob_Integer>(l->m_value == r->m_value);
    }
    if (op == TokenType::BANG_EQUAL)
    {
        return std::make_shared<Ob_Integer>(l->m_value != r->m_value);
    }
    if (op == TokenType::LESS)
    {
        return std::make_shared<Ob_Integer>(l->m_value < r->m_value);
    }
    if (op == TokenType::GREATER)
    {
        return std::make_shared<Ob_Integer>(l->m_value > r->m_value);
    }
    if (op == TokenType::LESS_EQUAL)
    {
        return std::make_shared<Ob_Integer>(l->m_value <= r->m_value);
    }
    if (op == TokenType::GREATER_EQUAL)
    {
        return std::make_shared<Ob_Integer>(l->m_value >= r->m_value);
    }

    return new_error("Evaluator::eval_integer_infix_expression unknown operation: %s %s %s", left->name().c_str(), TokenTypeToString[op].c_str(), right->name().c_str());
}
std::shared_ptr<Object> Evaluator::eval_fraction_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                                  const std::shared_ptr<Object> &right)
{
    auto l = std::dynamic_pointer_cast<Ob_Fraction>(left);
    auto r = std::dynamic_pointer_cast<Ob_Fraction>(right);
    if (op == TokenType::PLUS)
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::add(l, r));
    }
    if (op == TokenType::MINUS)
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::sub(l, r));
    }
    if (op == TokenType::STAR)
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::mul(l, r));
    }
    if (op == TokenType::SLASH)
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::div(l, r));
    }
    if (op == TokenType::PERCENT)
    {
        return std::make_shared<Ob_Fraction>(Ob_Fraction::mod(l, r));
    }

    return new_error("Evaluator::eval_fraction_infix_expression unknown operation: %s %s %s", left->name().c_str(), TokenTypeToString[op].c_str(), right->name().c_str());
}
std::shared_ptr<Object> Evaluator::eval_assign_expression(const std::shared_ptr<Object> &name, const std::shared_ptr<Object> &value)
{
    std::cout << "eval_assign_expression: " << name->str() << " = " << value->str() << std::endl;
    return value;
}
