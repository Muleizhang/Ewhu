#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_integer(const std::shared_ptr<Integer> &node)
{
    return new_integer(node->m_value);
}