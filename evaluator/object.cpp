#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_identifier(const std::shared_ptr<Node> &node, Scope &scp)
{
    auto it = scp.m_var.find(node->m_name);
    if (it != scp.m_var.end())
    {
        return it->second->clone();
    }

    auto current_scope = &scp;
    while (current_scope->father != nullptr)
    {
        current_scope = current_scope->father;
        auto it = current_scope->m_var.find(node->m_name);
        if (it != current_scope->m_var.end())
        {
            return it->second->clone();
        }
    }

    return new_error("Evaluator::eval_identifier: identifier not found: %d", node->m_name);
}

std::shared_ptr<Object> Evaluator::eval_identifier_self(const std::shared_ptr<Node> &node, Scope &scp)
{
    auto it = scp.m_var.find(node->m_name);
    if (it != scp.m_var.end())
    {
        return it->second;
    }

    auto current_scope = &scp;
    while (current_scope->father != nullptr)
    {
        current_scope = current_scope->father;
        auto it = current_scope->m_var.find(node->m_name);
        if (it != current_scope->m_var.end())
        {
            return it->second;
        }
    }

    return new_error("Evaluator::eval_identifier: identifier not found: %d", node->m_name);
}