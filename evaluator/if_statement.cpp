#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result = nullptr;
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
        break;
    }
    return result;
}