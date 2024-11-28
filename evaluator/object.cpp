#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_identifier(const std::shared_ptr<Identifier> &node, Scope &scp)
{
    auto it = scp.m_var.find(node->m_name);
    if (it != scp.m_var.end())
    {
        return it->second;
    }
    else
        return new_error("Evaluator::eval_identifier: identifier not found: %s", node->m_name.c_str());
}

std::shared_ptr<Object> Evaluator::eval_new_identifier(const std::shared_ptr<Identifier> &node)
{

    return new_identifier(node->m_name);
}

std::shared_ptr<Object> Evaluator::eval_integer(const std::shared_ptr<Integer> &node)
{
    return new_integer(node->m_value);
}

std::shared_ptr<Object> Evaluator::eval_fraction(const std::shared_ptr<Object> &numerator,
                                                 const std::shared_ptr<Object> &denominator)
{
    auto l = std::dynamic_pointer_cast<Ob_Integer>(numerator);
    auto r = std::dynamic_pointer_cast<Ob_Integer>(denominator);
    std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_value, r->m_value));
    return s;
}
