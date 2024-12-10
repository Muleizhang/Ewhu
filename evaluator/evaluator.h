#pragma once
#include <list>
#include <memory>
#include <unordered_map>
#include "scope.h"
#include "../ast/node.h"
#include "../ast/statement.h"
#include "../ast/infix.h"
#include "../object/object.h"

class Evaluator
{
private:
    Scope scope;

public:
    Evaluator() {}
    ~Evaluator() {}

    bool is_error(const std::shared_ptr<Object> &obj);
    std::shared_ptr<Object> new_integer(__INT64_TYPE__ value);
    std::shared_ptr<Object> new_identifier(const std::string &value);
    std::shared_ptr<Object> new_error(const char *format, ...);

    std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, Scope &scp); // 求值
    // std::shared_ptr<Object> eval_left(const std::shared_ptr<Node> &node, Scope &scp);                             // 对左值特殊处理
    std::shared_ptr<Object> eval_program(const std::vector<std::shared_ptr<Statement>> &node, Scope &global_scp); // 对根节点求值
    std::shared_ptr<Object> eval_integer(const std::shared_ptr<Integer> &node);                                   // 求值

    std::shared_ptr<Object> eval_statement_block(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &scp); // 对语句块求值
    // std::shared_ptr<Object> eval_function_block(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &temp_scp); // 对函数语句块求值

    std::shared_ptr<Object> eval_if_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp);    // 对语句块求值
    std::shared_ptr<Object> eval_while_statement(const std::shared_ptr<Expression> &exp, const std::shared_ptr<Statement> true_statement, Scope &scp); // 对语句块求值
    std::shared_ptr<Object> eval_new_identifier(const std::shared_ptr<Identifier> &node);
    std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Identifier> &node, Scope &scp); // 求值
    std::shared_ptr<Object> eval_fraction(const std::shared_ptr<Object> &left,
                                          const std::shared_ptr<Object> &right);
    std::shared_ptr<Object> &eval_index(std::shared_ptr<Object> &name,
                                        const std::shared_ptr<Object> &index, Scope &scp);
    std::shared_ptr<Object> eval_assgin_array_statement(const std::shared_ptr<Node> &node, Scope &scp);
    std::shared_ptr<Object> eval_function_declaration(const std::shared_ptr<Function> &node, Scope &scp);
    std::shared_ptr<Object> eval_function(const std::shared_ptr<FunctionIdentifier> &node, Scope &scp);
    std::shared_ptr<Object> eval_return_statement(const std::shared_ptr<ExpressionStatement> &node, Scope &scp);
    std::shared_ptr<Object> eval_eval(const std::string &line, Scope &scp);
    std::shared_ptr<Object> eval_assign_expression(const std::string &name,
                                                   const std::shared_ptr<Object> &value, Scope &scp); // 赋值
    std::shared_ptr<Object> eval_infix(const TokenType op, std::shared_ptr<Object> &left,
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
