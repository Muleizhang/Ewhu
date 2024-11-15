#pragma once
#include "node.h"
#include <memory>
class ExpressionStatement : public Node
{
public:
    ExpressionStatement() : Node(Type::NODE_EXPRESSION_STATEMENT) {}
    ~ExpressionStatement() {}

public:
    std::shared_ptr<ExpressionStatement> m_expression;
};