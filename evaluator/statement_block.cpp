#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_statement_block(const std::list<std::shared_ptr<Statement>> &stmts, Scope &scp)
{
    std::shared_ptr<Object> result;
    Scope temp_scope(scp.m_var); // 局部作用域
    for (auto &stat : stmts)
    {
        result = eval(stat, temp_scope);
        if (is_error(result))
        {
            break;
        }
    }
    for (auto it : temp_scope.m_var)
    {
        auto org_it = scp.m_var.find(it.first);
        if (org_it != scp.m_var.end())
            scp.m_var[it.first] = it.second;
    }
    return result;
}