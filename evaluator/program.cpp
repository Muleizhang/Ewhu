#include "evaluator.h"

std::shared_ptr<Object> Evaluator::eval_program(const std::list<std::shared_ptr<Statement>> &stmts, Scope &global_scp)
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