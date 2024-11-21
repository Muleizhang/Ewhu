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
    else
    {
        return new_error("unknown operator: %s %s %s", left->name().c_str(), "operator", right->name().c_str());
    }
}