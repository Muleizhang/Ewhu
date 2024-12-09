#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_statement_block(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Null());
    std::shared_ptr<Object> returnvalue(new Ob_Null());
    Scope temp_scope(scp.m_var, scp.m_func); // 局部作用域
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

    for (auto it : temp_scope.m_var)
    {
        auto org_it = scp.m_var.find(it.first);
        if (org_it != scp.m_var.end())
            scp.m_var[it.first] = it.second;
    }

    return returnvalue;
}

std::shared_ptr<Object> Evaluator::eval_function_declaration(const std::shared_ptr<Function> &node, Scope &scp)
{
    scp.m_func.insert(std::make_pair((node->m_name)->m_name, std::dynamic_pointer_cast<Function>(node)));
    return std::make_shared<Ob_Null>(Ob_Null());
}

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Null());
    std::shared_ptr<Object> returnvalue(new Ob_Null());
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
    if (is_error(result) || result->type() == Object::OBJECT_RETURN)
        returnvalue = result;
    return returnvalue;
}

std::shared_ptr<Object> Evaluator::eval_while_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result(new Ob_Null());
    std::shared_ptr<Object> returnvalue(new Ob_Null());
    auto s = eval(exp, scp);

    switch (s->type())
    {
    case Object::OBJECT_BOOLEAN:
        if (std::dynamic_pointer_cast<Ob_Boolean>(s)->m_value)
        {
            do
            {
                result = eval(true_statement, scp);
                if (result->type() == Object::OBJECT_BREAK)
                {
                    result.reset(new Ob_Null());
                    return returnvalue;
                }
                if (result->type() == Object::OBJECT_CONTINUE)
                {
                    result.reset(new Ob_Null);
                    continue;
                }
                if (result->type() == Object::OBJECT_RETURN)
                {
                    returnvalue = result;
                    return returnvalue;
                }
            } while (std::dynamic_pointer_cast<Ob_Boolean>(eval(exp, scp))->m_value);
        }
        break;

    case Object::OBJECT_INTEGER:
        if (std::dynamic_pointer_cast<Ob_Integer>(s)->m_value)
        {

            do
            {
                result = eval(true_statement, scp);
                if (result->type() == Object::OBJECT_BREAK)
                {
                    result.reset(new Ob_Null());
                    return returnvalue;
                }
                if (result->type() == Object::OBJECT_CONTINUE)
                {
                    result.reset(new Ob_Null);
                    continue;
                }
                if (result->type() == Object::OBJECT_RETURN)
                {
                    returnvalue = result;
                    return returnvalue;
                }
            } while (std::dynamic_pointer_cast<Ob_Integer>(eval(exp, scp))->m_value);
        }
        break;

    case Object::OBJECT_FRACTION:
        if (std::dynamic_pointer_cast<Ob_Fraction>(s)->num)
        {
            do
            {
                result = eval(true_statement, scp);
                if (result->type() == Object::OBJECT_BREAK)
                {
                    result.reset(new Ob_Null());
                    return returnvalue;
                }
                if (result->type() == Object::OBJECT_CONTINUE)
                {
                    result.reset(new Ob_Null);
                    continue;
                }
                if (result->type() == Object::OBJECT_RETURN)
                {
                    returnvalue = result;
                    return returnvalue;
                }
            } while (std::dynamic_pointer_cast<Ob_Fraction>(eval(exp, scp))->num);
        }
        break;

    default:
        new_error("Evaluator::eval_while_statement: unknown type: %d", s->type());
        break;
    }
    if (is_error(result))
        returnvalue = result;
    return returnvalue;
}

std::shared_ptr<Object> Evaluator::eval_return_statement(const std::shared_ptr<ExpressionStatement> &node, Scope &scp)
{
    std::shared_ptr<Ob_Return> e(new Ob_Return());
    e->m_expression = eval(std::dynamic_pointer_cast<ReturnStatement>(node)->m_expression, scp);
    return e;
}