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
            {
                if (node->m_left->type() == Node::NODE_INFIX && node->m_left->m_operator == TokenType::LEFT_BRACKET)
                {
                    int idex = eval(node->m_left->m_right, scp)->m_int;
                    if (node->m_left->m_left->type() == Node::NODE_IDENTIFIER)
                    {
                        auto it = scp.m_var.find(node->m_left->m_left->m_name);
                        if (it != scp.m_var.end())
                            return (it->second)->m_array[idex] = eval(node->m_right, scp);

                        auto current_scope = &scp;
                        while (current_scope->father != nullptr)
                        {
                            current_scope = current_scope->father;
                            auto it = current_scope->m_var.find(node->m_left->m_left->m_name);
                            if (it != current_scope->m_var.end())
                            {
                                auto a = eval(node->m_right, scp);
                                return (it->second)->m_array[idex] = a;
                            }
                        }
                    }
                    else
                    {
                        auto ay = eval_assign_array(node->m_left->m_left, scp);
                        if (ay->type() == Object::OBJECT_ERROR)
                            return ay;
                        return ay->m_array[idex] = eval(node->m_right, scp);
                    }
                }
                return new_error("Evaluator::eval_left: not an identifier: ");
            }

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
        return eval_prefix(node->m_operator, node->m_right, scp);
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
        auto returnvalue = std::make_shared<Ob_Return>();
        returnvalue->m_expression = eval(node->m_expression_statement, scp);
        return returnvalue;
    }
    case Node::NODE_ARRAY:
    {
        auto ary = std::make_shared<Ob_Array>();
        for (auto ele : std::dynamic_pointer_cast<Array>(node)->m_array)
        {
            ary->Object::m_array.push_back(eval(ele, scp));
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

    auto obj = std::make_shared<Ob_Error>();
    obj->m_messages = buf;
    return obj;
}

std::shared_ptr<Object> Evaluator::eval_program(const std::vector<std::shared_ptr<Node>> &stmts, Scope &global_scp)
{
    std::shared_ptr<Object> result = eval(*(--stmts.end()), global_scp);
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
std::shared_ptr<Object> Evaluator::eval_assign_array(const std::shared_ptr<Node> node, Scope &scp)
{
    if (node->type() == Node::NODE_IDENTIFIER)
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
        return new_error("Evaluator::eval_assign_array: identifier %s not found", node->m_name);
    }
    if (node->type() == Node::NODE_INFIX && node->m_operator == TokenType::LEFT_BRACKET)
    {
        int idex = eval(node->m_right, scp)->m_int;
        if (node->m_left->type() == Node::NODE_IDENTIFIER)
        {
            auto it = scp.m_var.find(node->m_name);
            if (it != scp.m_var.end())
            {
                return it->second->m_array[idex];
            }

            auto current_scope = &scp;
            while (current_scope->father != nullptr)
            {
                current_scope = current_scope->father;
                auto it = current_scope->m_var.find(node->m_name);
                if (it != current_scope->m_var.end())
                {
                    return it->second->m_array[idex];
                }
            }
            return new_error("Evaluator::eval_assign_array: identifier %s not found", node->m_left->m_name);
        }
        else
        {
            auto array = eval_assign_array(node->m_left, scp);
            if (array->type() == Object::OBJECT_ERROR)
                return array;
            return array->m_array[idex];
        }
    }
    else
        return new_error("Evaluator: type error");
}