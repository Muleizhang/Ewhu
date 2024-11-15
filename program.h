#pragma once
#include "node.h"
#include <memory>
#include <list>
class Program : public Statement // 根节点
{
public:
    Program() : Statement(Type::NODE_PROGRAM) {}
    ~Program() {}

public:
    std::list<std::shared_ptr<Statement>> m_statements;
};