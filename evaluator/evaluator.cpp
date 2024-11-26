#pragma once
#include "evaluator.h"

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
    return Object::new_integer(value);
}
std::shared_ptr<Object> Evaluator::new_fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator)
{
    return Object::new_fraction(numerator, denominator);
}
std::shared_ptr<Object> Evaluator::new_identifier(const std::string &value)
{
    return Object::new_identifier(value);
}
std::shared_ptr<Object> Evaluator::eval(const std::shared_ptr<Node> &node)
{
    switch (node->type())
    {
    case Node::NODE_PROGRAM:
    {
        auto s = std::dynamic_pointer_cast<Program>(node); // 类型转换
        if (s->m_statements.empty())
            return nullptr;
        return eval_program(s->m_statements);
    }
    case Node::NODE_EXPRESSION_STATEMENT:
    {
        auto s = std::dynamic_pointer_cast<ExpressionStatement>(node);
        return eval(s->m_expression);
    }
    case Node::NODE_IDENTIFIER:
    {
        auto e = std::dynamic_pointer_cast<Identifier>(node);
        return eval_identifier(e);
    }
    case Node::NODE_BOOLEAN:
    {
        auto e = std::dynamic_pointer_cast<Boolean>(node);
        return Object::new_boolean(e->m_value);
    }
    case Node::NODE_INTEGER:
    {
        auto e = std::dynamic_pointer_cast<Integer>(node);
        return eval_integer(e);
    }
    case Node::NODE_INFIX:
    {
        auto e = std::dynamic_pointer_cast<Infix>(node);
        auto left = eval(e->m_left);
        if (is_error(left))
        {
            return left;
        }
        auto right = eval(e->m_right);
        if (is_error(right))
        {
            return right;
        }
        return eval_infix(e->m_operator, left, right);
    }
    case Node::NODE_PREFIX:
    {
        auto e = std::dynamic_pointer_cast<Prefix>(node);
        auto right = eval(e->m_right);
        return eval_prefix(e->m_operator, right);
    }
    default:
    {
        return new_error("Evaluator: node type error");
        break; // error
    }
    }
}