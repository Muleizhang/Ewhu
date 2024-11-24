#pragma once
#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_identifier(const std::shared_ptr<Identifier> &node)
{
    return new_identifier(node->m_name);
}