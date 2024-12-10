#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_statement_block(const std::vector<std::shared_ptr<Node>> &stmts, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Null());
    std::shared_ptr<Object> returnvalue(new Ob_Null());
    Scope temp_scope; // 局部作用域
    temp_scope.father = &scp;
    for (auto &stat : stmts)
    {
        result = eval(stat, temp_scope);
        if (result->type() == Object::OBJECT_BREAK || result->type() == Object::OBJECT_CONTINUE)
        {
            break;
        }
        if (is_error(result) || result->type() == Object::OBJECT_RETURN)
        {
            returnvalue = result;
            break;
        }
    }

    // for (auto it : temp_scope.m_var)
    // {
    //     auto org_it = scp.m_var.find(it.first);
    //     if (org_it != scp.m_var.end())
    //         scp.m_var[it.first] = it.second;
    // }

    return returnvalue;
}

std::shared_ptr<Object> Evaluator::eval_function_declaration(const std::shared_ptr<Node> &node, Scope &scp)
{
    scp.m_func.insert(std::make_pair((node->m_func)->m_name, node));
    return std::make_shared<Ob_Null>();
}

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Node> &exp, const std::shared_ptr<Node> true_statement, Scope &scp)
{
    if (eval(exp, scp)->m_int)
    {
        std::shared_ptr<Object> result = eval(true_statement, scp);
        if (is_error(result) || result->type() == Object::OBJECT_RETURN)
            return result;
    }
    return std::make_shared<Ob_Null>();
}

std::shared_ptr<Object> Evaluator::eval_while_statement(const std::shared_ptr<Node> &exp, const std::shared_ptr<Node> true_statement, Scope &scp)
{
    auto s = eval(exp, scp);

    while (s->m_int)
    {
        std::shared_ptr<Object> result = eval(true_statement, scp);
        if (result->type() == Object::OBJECT_BREAK)
        {
            return std::make_shared<Ob_Null>();
        }
        if (result->type() == Object::OBJECT_CONTINUE)
        {
            continue;
        }
        if (result->type() == Object::OBJECT_RETURN || is_error(result))
        {
            return result;
        }
        s = eval(exp, scp);
    }

    return std::make_shared<Ob_Null>();
}

std::shared_ptr<Object> Evaluator::eval_return_statement(const std::shared_ptr<Node> &node, Scope &scp)
{
    std::shared_ptr<Ob_Return> e(new Ob_Return());
    e->m_expression = eval(std::dynamic_pointer_cast<ReturnStatement>(node)->m_expression_statement, scp);
    return e;
}