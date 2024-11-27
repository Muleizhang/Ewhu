#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp)
{
    std::shared_ptr<Object> result = nullptr;
    auto s = eval(exp, scp);
    if (std::dynamic_pointer_cast<Ob_Boolean>(s)->m_value)
    {
        result = eval(true_statement, scp);
    }
    return result;
}