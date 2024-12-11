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

    bool is_error(const std::shared_ptr<Object> &obj);          // 是否是错误
    std::shared_ptr<Object> new_error(const char *format, ...); // 输出错误

    std::shared_ptr<Object> eval(const std::shared_ptr<Node> &node, Scope &scp); // 求值
    // std::shared_ptr<Object> eval_left(const std::shared_ptr<Node> &node, Scope &scp);                             // 对左值特殊处理
    std::shared_ptr<Object> eval_program(const std::vector<std::shared_ptr<Node>> &node, Scope &global_scp); // 对根节点求值

    std::shared_ptr<Object> eval_statement_block(const std::vector<std::shared_ptr<Node>> &stmts, Scope &scp); // 对语句块求值
    std::shared_ptr<Object> eval_function_block(const std::shared_ptr<Node> function,
                                                std::shared_ptr<Node> node, Scope &scp); // 对函数语句块求值

    // std::shared_ptr<Object> eval_function_block(const std::vector<std::shared_ptr<Statement>> &stmts, Scope &temp_scp); // 对函数语句块求值
    std::shared_ptr<Object> eval_assign_array(const std::shared_ptr<Node>, Scope &scp);                                                     // 对数组求值
    std::shared_ptr<Object> eval_if_statement(const std::shared_ptr<Node> &exp, const std::shared_ptr<Node> true_statement, Scope &scp);    // 对语句块求值
    std::shared_ptr<Object> eval_while_statement(const std::shared_ptr<Node> &exp, const std::shared_ptr<Node> true_statement, Scope &scp); // 对语句块求值
    std::shared_ptr<Object> eval_identifier(const std::shared_ptr<Node> &node, Scope &scp);                                                 // 对标识符求值
    std::shared_ptr<Object> eval_function_declaration(const std::shared_ptr<Node> &node, Scope &scp);                                       // 对函数声明求值
    std::shared_ptr<Object> eval_function(const std::shared_ptr<Node> &node, Scope &scp);                                                   // 对函数调用求值
    std::shared_ptr<Object> eval_return_statement(const std::shared_ptr<Node> &node, Scope &scp);                                           // 对返回语句求值

    std::shared_ptr<Object> eval_index(std::shared_ptr<Object> &name,
                                       const std::shared_ptr<Object> &index, Scope &scp); // 对数组索引求值
    std::shared_ptr<Object> eval_assign_expression(const std::string &name,
                                                   const std::shared_ptr<Object> &value, Scope &scp); // 赋值语句
    std::shared_ptr<Object> eval_infix(const TokenType op, std::shared_ptr<Object> &left,
                                       const std::shared_ptr<Object> &right, Scope &Scp); // 对中缀表达式求值
    std::shared_ptr<Object> eval_integer_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                          const std::shared_ptr<Object> &right); // 整数中缀表达式
    std::shared_ptr<Object> eval_fraction_infix_expression(const TokenType &op, const std::shared_ptr<Object> &left,
                                                           const std::shared_ptr<Object> &right);                       // 分数中缀表达式
    std::shared_ptr<Object> eval_prefix(const TokenType &op, const std::shared_ptr<Object> &right);                     // 对前缀表达式求值
    std::shared_ptr<Object> eval_integer_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);  // 对整数前缀表达式求值
    std::shared_ptr<Object> eval_fraction_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right); // 对分数前缀表达式求值
    std::shared_ptr<Object> eval_boolean_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);  // 对布尔前缀表达式求值
    std::shared_ptr<Object> eval_trignometry_prefix_expression(const TokenType &op, const std::shared_ptr<Object> &right);

    /*内置函数*/
    std::shared_ptr<Object> eval_eval(const std::string &line, Scope &scp);             // 对eval函数求值
    std::shared_ptr<Object> eval_append(const std::shared_ptr<Node> &node, Scope &scp); // 对append函数求值
    std::shared_ptr<Object> eval_pop(const std::shared_ptr<Node> &node, Scope &scp);    // 对pop函数求值
    std::shared_ptr<Object> eval_int(const std::shared_ptr<Node> &node, Scope &scp);    // 对int类型转化函数求值
    // std::shared_ptr<Object> eval_ast();
};
