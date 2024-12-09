#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_identifier(const std::shared_ptr<Node> &node, Scope &scp)
{
    auto it = scp.m_var.find(node->m_name);
    if (it != scp.m_var.end())
    {
        // std::cout << it->second->str() << std::endl;
        return it->second->clone();
    }
    else
        return new_error("Evaluator::eval_identifier: identifier not found: %s", node->m_name.c_str());
}
