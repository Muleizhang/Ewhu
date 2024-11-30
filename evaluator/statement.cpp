#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_statement_block(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Empty());
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

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Empty());
    auto s = eval(exp, scp);
    switch (s->type())
    {
    case Object::OBJECT_BOOLEAN:
        if (std::dynamic_pointer_cast<Ob_Boolean>(s)->m_value)
            result = eval(true_statement, scp);
        break;

    case Object::OBJECT_INTEGER:
        if (std::dynamic_pointer_cast<Ob_Integer>(s)->m_value)
            result = eval(true_statement, scp);
        break;

    case Object::OBJECT_FRACTION:
        if (std::dynamic_pointer_cast<Ob_Fraction>(s)->num)
            result = eval(true_statement, scp);
        break;
    default:
        new_error("Evaluator::eval_if_statement: unknown type: %d", s->type());
        break;
    }
    return result;
}

std::shared_ptr<Object> Evaluator::eval_while_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result = nullptr;
    auto s = eval(exp, scp);
    switch (s->type())
    {
    case Object::OBJECT_BOOLEAN:
        while (std::dynamic_pointer_cast<Ob_Boolean>(eval(exp, scp))->m_value)
        {
            result = eval(true_statement, scp);
            if (result->type() == Object::OBJECT_BREAK)
                break;
        }
        break;

    case Object::OBJECT_INTEGER:
        while (std::dynamic_pointer_cast<Ob_Integer>(eval(exp, scp))->m_value)
        {
            result = eval(true_statement, scp);
            if (result->type() == Object::OBJECT_BREAK)
                break;
        }
        break;

    case Object::OBJECT_FRACTION:
        while (std::dynamic_pointer_cast<Ob_Fraction>(eval(exp, scp))->num)
        {
            result = eval(true_statement, scp);
            if (result->type() == Object::OBJECT_BREAK)
                break;
        }

        break;
    default:
        new_error("Evaluator::eval_while_statement: unknown type: %d", s->type());
        break;
    }
    return result;
}
