#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval(const std::shared_ptr<Node> &node, Scope &scp)
{
    switch (node->type())
    {
    case Node::NODE_PROGRAM:
    {
        auto s = std::dynamic_pointer_cast<Program>(node); // 类型转换
        if (s->m_statements.empty())
            return nullptr;
        return eval_program(s->m_statements, scp);
    }
    case Node::NODE_STATEMENTBLOCK:
    {
        auto s = std::dynamic_pointer_cast<StatementBlock>(node);
        return eval_statement_block(s->m_statements, scp);
    }
    case Node::NODE_IFSTATEMENT:
    {
        auto s = std::dynamic_pointer_cast<IfStatement>(node);
        return eval_if_statement(s->m_expression, s->m_true_statement, scp);
    }
    case Node::NODE_WHILESTATEMENT:
    {
        auto s = std::dynamic_pointer_cast<WhileStatement>(node);
        return eval_while_statement(s->m_expression, s->m_cycle_statement, scp);
    }
    case Node::NODE_EXPRESSION_STATEMENT:
    {
        auto s = std::dynamic_pointer_cast<ExpressionStatement>(node);
        return eval(s->m_expression, scp);
    }
    case Node::NODE_IDENTIFIER:
    {
        auto e = std::dynamic_pointer_cast<Identifier>(node);
        return eval_identifier(e, scp);
    }
    case Node::NODE_BOOLEAN:
    {
        auto e = std::dynamic_pointer_cast<Boolean>(node);
        return std::make_shared<Ob_Boolean>(e->m_value);
    }
    case Node::NODE_INTEGER:
    {
        auto e = std::dynamic_pointer_cast<Integer>(node);
        return eval_integer(e);
    }
    case Node::NODE_STRING:
    {
        auto e = std::dynamic_pointer_cast<String>(node);
        return std::make_shared<Ob_String>(e->m_value);
    }
    case Node::NODE_INFIX:
    {
        auto e = std::dynamic_pointer_cast<Infix>(node);
        std::shared_ptr<Object> left;
        if (e->m_operator != TokenType::EQUAL)
        {
            left = eval(e->m_left, scp);
        }
        else
        {
            left = eval_left(e->m_left, scp);
        }
        if (is_error(left))
        {
            return left;
        }
        auto right = eval(e->m_right, scp);
        if (is_error(right))
        {
            return right;
        }
        return eval_infix(e->m_operator, left, right, scp);
    }
    case Node::NODE_PREFIX:
    {
        auto e = std::dynamic_pointer_cast<Prefix>(node);
        auto right = eval(e->m_right, scp);
        return eval_prefix(e->m_operator, right);
    }
    case Node::NODE_BREAKSTATEMENT:
    {
        std::shared_ptr<Ob_Break> e(new Ob_Break);
        return e;
    }
    case Node::NODE_CONTINUESTATEMENT:
    {
        std::shared_ptr<Ob_Continue> e(new Ob_Continue);
        return e;
    }
    case Node::NODE_FUNCTION:
    {
        auto e = std::dynamic_pointer_cast<Function>(node);
        return eval_function_declaration(e, scp);
    }
    case Node::NODE_FUNCTION_IDENTIFIER:
    {
        auto e = std::dynamic_pointer_cast<FunctionIdentifier>(node);
        return eval_function(e, scp);
    }

    default:
    {
        return new_error("Evaluator: node type error: %d", Node::m_names[node->type()]);
        break;
    }
    }
}

bool Evaluator::is_error(const std::shared_ptr<Object> &obj)
{
    return obj->type() == Object::OBJECT_ERROR;
}

std::shared_ptr<Object> Evaluator::new_error(const char *format, ...)
{
    char buf[114514] = {0};
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(buf, sizeof(buf), format, arg_ptr);
    va_end(arg_ptr);

    std::shared_ptr<Ob_Error> obj(new Ob_Error());
    obj->m_messages = buf;
    return obj;
}

std::shared_ptr<Object> Evaluator::new_integer(__INT64_TYPE__ value)
{
    return std::make_shared<Ob_Integer>(value);
}

std::shared_ptr<Object> Evaluator::new_identifier(const std::string &value)
{
    return std::make_shared<Ob_Identifier>(value);
}

std::shared_ptr<Object> Evaluator::eval_left(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (node->type() != Node::NODE_IDENTIFIER)
        return Evaluator::eval(node, scp);
    else
    {
        auto e = std::dynamic_pointer_cast<Identifier>(node);
        return eval_new_identifier(e);
    }
}

std::shared_ptr<Object> Evaluator::eval_program(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &global_scp)
{
    std::shared_ptr<Object> result;

    result = eval(*(--stmts.end()), global_scp);
    // for (auto &stat : stmts)
    // {
    //     result = eval(stat);
    //     if (is_error(result))
    //     {
    //         break;
    //     }
    // }
    return result;
}
