#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval(const std::shared_ptr<Node> &node, Scope &scp)
{
    switch (node->type())
    {
    case Node::NODE_PROGRAM:
    {
        if (node->m_statements.empty())
            return nullptr;
        return eval_program(node->m_statements, scp);
    }
    case Node::NODE_STATEMENTBLOCK:
    {
        return eval_statement_block(node->m_statements, scp);
    }
    case Node::NODE_IFSTATEMENT:
    {
        return eval_if_statement(node->m_expression, node->m_true_statement, scp);
    }
    case Node::NODE_WHILESTATEMENT:
    {
        return eval_while_statement(node->m_expression, node->m_cycle_statement, scp);
    }
    case Node::NODE_EXPRESSION_STATEMENT:
    {
        return eval(node->m_expression, scp);
    }
    case Node::NODE_IDENTIFIER:
    {
        return eval_identifier(node, scp);
    }
    case Node::NODE_BOOLEAN:
    {
        return std::make_shared<Ob_Boolean>(node->m_bool);
    }
    case Node::NODE_INTEGER:
    {
        return std::make_shared<Ob_Integer>(node->m_value);
    }
    case Node::NODE_STRING:
    {
        return std::make_shared<Ob_String>(node->m_string);
    }
    case Node::NODE_INFIX:
    {
        if (node->m_operator == TokenType::EQUAL)
        {
            if (node->m_left->type() != Node::NODE_IDENTIFIER)
                return new_error(("Evaluator::eval_left: not an identifier: " + Node::m_names[node->m_left->type()]).c_str());
            else
            {
                auto name = node->m_left->m_name;
                return eval_assign_expression(name, eval(node->m_right, scp), scp);
            }
        }
        else
        {
            auto left = eval(node->m_left, scp);
            auto right = eval(node->m_right, scp);
            return eval_infix(node->m_operator, left, right, scp);
        }
    }
    case Node::NODE_PREFIX:
    {
        return eval_prefix(node->m_operator, eval(node->m_right, scp));
    }
    case Node::NODE_BREAKSTATEMENT:
    {
        return std::make_shared<Ob_Break>();
    }
    case Node::NODE_CONTINUESTATEMENT:
    {
        return std::make_shared<Ob_Continue>();
    }
    case Node::NODE_FUNCTION:
    {
        return eval_function_declaration(node, scp);
    }
    case Node::NODE_FUNCTION_IDENTIFIER:
    {
        return eval_function(node, scp);
    }
    case Node::NODE_RETURNSTATEMENT:
    {
        std::shared_ptr<Ob_Return> returnvalue(new Ob_Return);
        returnvalue->m_expression = eval(node->m_expression_statement, scp);
        return returnvalue;
    }
    case Node::NODE_ARRAY:
    {
        std::shared_ptr<Ob_Array> ary(new Ob_Array);
        for (auto ele : std::dynamic_pointer_cast<Array>(node)->m_array)
        {
            ary->m_array.push_back(eval(ele, scp));
        }
        return ary;
    }

    default:
    {
        return new_error(("Evaluator: node type error: " + Node::m_names[node->type()]).c_str());
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

std::shared_ptr<Object> Evaluator::eval_program(const std::vector<std::shared_ptr<Node>> &stmts, Scope &global_scp)
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
