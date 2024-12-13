#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_statement_block(const std::vector<std::shared_ptr<Node>> &stmts, Scope &scp)
{
    std::shared_ptr<Object> result = nullptr;
    Scope temp_scope(&scp);
    for (auto &stat : stmts)
    {
        result = eval(stat, temp_scope);
        if (result)
        {
            if (result->type() == Object::OBJECT_BREAK || result->type() == Object::OBJECT_CONTINUE ||
                result->type() == Object::OBJECT_RETURN)
            {
                return result;
            }
        }
    }
    return result;
}

std::shared_ptr<Object> Evaluator::eval_function_block(const std::shared_ptr<Node> function,
                                                       std::shared_ptr<Node> node, Scope &scp)
{
    Scope temp_scp(&scp);
    if (function->m_initial_list.size() != node->m_initial_list.size())
    {
        throw std::runtime_error("Evaluator::eval_function: function arguments not match");
    }

    for (int i = 0; i < function->m_initial_list.size(); i++)
    {
        eval_assign_expression(function->m_initial_list[i]->m_name, eval(node->m_initial_list[i], scp), temp_scp);
    }

    std::shared_ptr<Object> result;
    for (auto &stat : function->m_statement->m_statements)
    {
        result = eval(stat, temp_scp);
        if (result)
        {
            if (result->type() == Object::OBJECT_BREAK || result->type() == Object::OBJECT_CONTINUE)
            {
                return nullptr;
            }
            if (result->type() == Object::OBJECT_RETURN)
            {
                return std::dynamic_pointer_cast<Ob_Return>(result)->m_expression;
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Object> Evaluator::eval_function_declaration(const std::shared_ptr<Node> &node, Scope &scp)
{
    scp.m_func.insert(std::make_pair((node->m_func)->m_name, node));
    return nullptr;
}

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (eval(node->m_expression, scp)->m_int)
    {
        auto result = eval(node->m_true_statement, scp);
        return result;
    }
    else if (node->m_false_statement)
    {
        auto result = eval(node->m_false_statement, scp);
        return result;
    }
    return nullptr;
}

std::shared_ptr<Object> Evaluator::eval_while_statement(const std::shared_ptr<Node> &exp, const std::shared_ptr<Node> true_statement, Scope &scp)
{
    auto s = eval(exp, scp);

    while (s->m_int)
    {
        std::shared_ptr<Object> result = eval(true_statement, scp);
        if (result)
        {
            if (result->type() == Object::OBJECT_BREAK)
            {
                return nullptr;
            }
            if (result->type() == Object::OBJECT_CONTINUE)
            {
                continue;
            }
            if (result->type() == Object::OBJECT_RETURN)
            {
                return result;
            }
        }
        s = eval(exp, scp);
    }

    return nullptr;
}

std::shared_ptr<Object> Evaluator::eval_return_statement(const std::shared_ptr<Node> &node, Scope &scp)
{
    return std::make_shared<Ob_Return>(eval(std::dynamic_pointer_cast<ReturnStatement>(node)->m_expression_statement, scp));
}

std::shared_ptr<Object> Evaluator::eval_append(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (node->m_initial_list.size() != 2)
        throw std::invalid_argument("Evaluator:eval_function: function append arguments not match");
    else
    {
        auto ele = eval(node->m_initial_list[1], scp);
        auto array = eval_assign_array(node->m_initial_list[0], scp);
        array->m_array.push_back(ele);
        return nullptr;
    }
}

std::shared_ptr<Object> Evaluator::eval_pop(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (node->m_initial_list.size() != 1)
        throw std::invalid_argument("Evaluator:eval_function: function pop arguments not match");
    else
    {
        auto array = eval_assign_array(node->m_initial_list[0], scp);
        if (array->m_array.empty())
            throw std::runtime_error("Evaluator:eval_pop: pop from empty array");
        auto top = array->m_array.back();
        array->m_array.pop_back();
        return top;
    }
}

std::shared_ptr<Object> Evaluator::eval_int(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (node->m_initial_list.size() != 1)
        throw std::invalid_argument("Evaluator:eval_function: function append arguments not match");
    auto obj = eval(node->m_initial_list[0], scp);
    switch (obj->type())
    {
    case Object::OBJECT_INTEGER:
        return obj;
    case Object::OBJECT_FRACTION:
        return std::make_shared<Ob_Integer>(obj->m_integerPart);
    case Object::OBJECT_BOOLEAN:
        return std::make_shared<Ob_Integer>(obj->m_int);
    case Object::OBJECT_STRING:
    {
        std::shared_ptr<Ob_Integer> newint;
        return std::make_shared<Ob_Integer>(std::stoll(obj->m_string));
    }
    default:
        throw std::runtime_error("Evaluator:eval_function: can not convert " + obj->name() + " to Integer");
    }
}
/*
std::shared_ptr<Object> Evaluator::eval_ast()
{
    rapidjson::Document root;
    root.SetObject();
    root.AddMember("program", program->json(root), root.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    root.Accept(writer);
    return std::make_shared<Ob_String>(buffer.GetString());
    //std::cout << "\033[32m" << "AST output to ast.jsonヾ(✿ﾟ▽ﾟ)ノ" << "\033[0m" << std::endl;
}*/
void read(char *inpt)
{
    int len = 0;
    char c = getchar();
    while (c == ' ')
        c = getchar();
    inpt[len++] = c;
    while (1)
    {
        c = getchar();
        if (c == '\n')
            break;
        inpt[len++] = c;
    }
    inpt[len] = '\0';
}

std::shared_ptr<Object> Evaluator::eval_input(const std::shared_ptr<Node> &node, Scope &scp)
{
    if (node->m_initial_list.size() != 1)
        throw std::invalid_argument("Evaluator:eval_function: function append arguments not match");
    std::shared_ptr<Object> otpt = eval(node->m_initial_list[0], scp);
    std::cout << (otpt->m_string);
    char inpt[1024];
    read(inpt);
    return std::make_shared<Ob_String>(inpt);
}