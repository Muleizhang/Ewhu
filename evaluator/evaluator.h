#pragma once
#include "../ast/head.h"
#include "../object/header.h"
#include <memory>
class Evaluator
{
public:
    Evaluator() {}
    ~Evaluator() {}

    bool is_error(const std::shared_ptr<Object> &obj);
    std::shared_ptr<Object> new_integer(__INT64_TYPE__ value);
    std::shared_ptr<Object> new_fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator);
    std::shared_ptr<Object> new_error(const char *format, ...);

    std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node);                         // 求值
    std::shared_ptr<Object> eval_program(const std::list<std::shared_ptr<Statement>> &node); // 对根节点求值
    std::shared_ptr<Object> eval_integer(const std::shared_ptr<Integer> &node);              // 求值
    std::shared_ptr<Object> eval_fraction(const std::shared_ptr<Object> &left,
                                          const std::shared_ptr<Object> &right); // 求值
    std::shared_ptr<Object> eval_infix(const TokenType op, const std::shared_ptr<Object> &left,
                                       const std::shared_ptr<Object> &right); // 求值
    std::shared_ptr<Object> eval_integer_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                          const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_fraction_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                           const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_prefix(const TokenType &op, const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_integer_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_fraction_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);
};