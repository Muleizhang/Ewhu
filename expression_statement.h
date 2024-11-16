#pragma once
#include "node.h"
#include <memory>
class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() : Statement(Type::NODE_EXPRESSION_STATEMENT) {}
    ~ExpressionStatement() {}

public:
    std::shared_ptr<Expression> m_expression;
};