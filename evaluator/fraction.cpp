#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_fraction(const std::shared_ptr<Object> &numerator,
                                                 const std::shared_ptr<Object> &denominator)
{
    auto l = std::dynamic_pointer_cast<Ob_Integer>(numerator);
    auto r = std::dynamic_pointer_cast<Ob_Integer>(denominator);
    std::shared_ptr<Ob_Fraction> s(new Ob_Fraction(l->m_value, r->m_value));
    return s;
}