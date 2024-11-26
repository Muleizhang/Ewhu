#pragma once
#include "../ast/head.h"
#include "../object/header.h"
#include <memory>
#include <unordered_map>
#include "scope.h"
class Evaluator
{
public:
    Evaluator() {}
    ~Evaluator() {}

    bool is_error(const std::shared_ptr<Object> &obj);
    std::shared_ptr<Object> new_integer(__INT64_TYPE__ value);
    std::shared_ptr<Object> new_fraction(__INT64_TYPE__ numerator, __INT64_TYPE__ denominator);
    std::shared_ptr<Object> new_identifier(const std::string &value);
    std::shared_ptr<Object> new_error(const char *format, ...);

    std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, Scope &scp);                                // 求值
    std::shared_ptr<Object> eval_left(const std::shared_ptr<Node> &node, Scope &scp);                // 对左值特殊处理
    std::shared_ptr<Object> eval_program(const std::list<std::shared_ptr<Statement>> &node, Scope &global_scp); // 对根节点求值
    std::shared_ptr<Object> eval_integer(const std::shared_ptr<Integer> &node);                                 // 求值

    std::shared_ptr<Object> eval_new_identifier(const std::shared_ptr<Identifier> &node);
    std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, Scope &scp); // 求值
    std::shared_ptr<Object> eval_fraction(const std::shared_ptr<Object> &left,
                                          const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_assign_expression(const std::shared_ptr<Object> &name,
                                                   const std::shared_ptr<Object> &value, Scope &scp); // 赋值
    std::shared_ptr<Object> eval_infix(const TokenType op, const std::shared_ptr<Object> &left,
                                       const std::shared_ptr<Object> &right, Scope &Scp); // 求值
    std::shared_ptr<Object> eval_integer_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                          const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_fraction_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                           const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_prefix(const TokenType &op, const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_integer_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_fraction_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> eval_boolean_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);
};