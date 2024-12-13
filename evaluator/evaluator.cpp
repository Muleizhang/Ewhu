#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval(const std::shared_ptr<Node> &node, Scope &scp)
{
    switch (node->type())
    {
    case Node::NODE_STATEMENTBLOCK:
    {
        return eval_statement_block(node->m_statements, scp);
    }
    case Node::NODE_IFSTATEMENT:
    {
        return eval_if_statement(node, scp);
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
            if (node->m_left->type() == Node::NODE_IDENTIFIER)
            {
                auto name = node->m_left->m_name;
                return eval_assign_expression(name, eval(node->m_right, scp), scp);
            }
            else
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
                        return ay->m_array[idex] = eval(node->m_right, scp);
                    }
                }
                throw std::runtime_error("Evaluator::eval_left: not an identifier: ");
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
        return std::make_shared<Ob_Return>(eval(node->m_expression_statement, scp));
    }
    case Node::NODE_ARRAY:
    {
        auto ary = std::make_shared<Ob_Array>();
        for (auto ele : std::dynamic_pointer_cast<Array>(node)->m_array)
        {
            ary->m_array.push_back(eval(ele, scp));
        }
        return ary;
    }

    default:
        throw std::invalid_argument("Evaluator: node type error: " + Node::m_names[node->type()]);
    }
}

std::shared_ptr<Object> Evaluator::eval_program(const std::shared_ptr<Program> &node, Scope &global_scp)
{
    function_map = node->function_map;
    identifier_map = node->identifier_map;

    if (node->m_statements.empty())
        throw std::runtime_error("Evaluator::eval: empty program");
    std::shared_ptr<Object> result = eval(*(--node->m_statements.end()), global_scp);
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
        throw std::runtime_error("Evaluator::eval_assign_arraaay: identifier not found");
    }
    if (node->type() == Node::NODE_INFIX && node->m_operator == TokenType::LEFT_BRACKET)
    {
        int idex = eval(node->m_right, scp)->m_int;
        if (node->m_left->type() == Node::NODE_IDENTIFIER)
        {
            auto it = scp.m_var.find(node->m_left->m_name);
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
            // node->m_left->m_name
            throw std::runtime_error("Evaluator::eval_assign_array: identifier  not found");
        }
        else
        {
            auto array = eval_assign_array(node->m_left, scp);
            if (array->type() == Object::OBJECT_ERROR)
                return array;
            return array->m_array[idex];
        }
    }
    throw std::runtime_error("Evaluator::eval_assign_array: type error");
}