#pragma once
#include "../ast/node.h"
#include "../ast/integer.h"
#include "../ast/identifier.h"
#include "../ast/infix.h"
#include "../ast/prefix.h"
#include "../ast/program.h"
#include "../ast/statement_block.h"
#include "../ast/if_statement.h"
#include "../ast/expression_statement.h"
#include "../ast/boolean.h"
#include "../object/boolean.h"
#include "../object/fraction.h"
#include "../object/integer.h"
#include "../object/string.h"
#include "../object/identifier.h"
#include "../object/object.h"
#include "../object/error.h"

#include <list>
#include <memory>
#include <unordered_map>
#include "scope.h"
class Evaluator
{
private:
    Scope scope;

public:
    Evaluator() {}
    ~Evaluator() {}

    bool is_error(const std::shared_ptr<Object> &obj);
    std::shared_ptr<Object> new_integer(long long value);
    std::shared_ptr<Object> new_fraction(long long numerator, long long denominator);
    std::shared_ptr<Object> new_identifier(const std::string &value);
    std::shared_ptr<Object> new_error(const char *format, ...);

    std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, Scope &scp);                                // 求值
    std::shared_ptr<Object> eval_left(const std::shared_ptr<Node> &node, Scope &scp);                           // 对左值特殊处理
    std::shared_ptr<Object> eval_program(const std::list<std::shared_ptr<Statement>> &node, Scope &global_scp); // 对根节点求值
    std::shared_ptr<Object> eval_integer(const std::shared_ptr<Integer> &node);                                 // 求值

    std::shared_ptr<Object> eval_statement_block(const std::list<std::shared_ptr<Statement>> &stmts, Scope &scp);                                   // 对语句块求值
    std::shared_ptr<Object> eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp); // 对语句块求值
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