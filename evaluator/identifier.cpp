#pragma once
#include "evaluator.h"
#include "scope.h"
std::shared_ptr<Object> Evaluator::eval_identifier(const std::shared_ptr<Identifier> &node, Scope &scp)
{
    auto it = scp.m_var.find(node->m_name);
    if (it != scp.m_var.end())
    {
        return it->second;
    }
    else
        return nullptr;
}
std::shared_ptr<Object> Evaluator::eval_new_identifier(const std::shared_ptr<Identifier> &node)
{

    return new_identifier(node->m_name);
}
