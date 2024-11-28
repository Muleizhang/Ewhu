#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_while_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result = nullptr;
    auto s = eval(exp, scp);
    switch (s->type())
    {
    case Object::OBJECT_BOOLEAN:
        while (std::dynamic_pointer_cast<Ob_Boolean>(eval(exp, scp))->m_value)
            result = eval(true_statement, scp);
        break;

    case Object::OBJECT_INTEGER:
        while (std::dynamic_pointer_cast<Ob_Integer>(eval(exp, scp))->m_value)
            result = eval(true_statement, scp);
        break;

    case Object::OBJECT_FRACTION:
        while (std::dynamic_pointer_cast<Ob_Fraction>(eval(exp, scp))->num)
            result = eval(true_statement, scp);
        break;

    default:
        new_error("Evaluator::eval_while_statement: unknown type: %d", s->type());
        break;
    }
    return result;
}