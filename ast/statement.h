#pragma once
#include "node.h"
#include "infix.h"

class StatementBlock : public Statement
{
public:
    StatementBlock() : Statement(Type::NODE_STATEMENTBLOCK) {}
    ~StatementBlock() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        for (auto &stat : m_statements)
        {
            statements.PushBack(stat->json(father), father.GetAllocator());
        }
        json.AddMember("statements", statements, father.GetAllocator());
        return json;
    }

public:
    std::vector<std::shared_ptr<Statement>> m_statements;
};

class Function : public Statement // 函数的声明
{
public:
    Function() : Statement(Type::NODE_FUNCTION) {}
    ~Function() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());

        rapidjson::Value args(rapidjson::kArrayType);
        for (auto &arg : m_initial_list)
        {
            args.PushBack(arg->json(father), father.GetAllocator());
        }
        json.AddMember("arguments", args, father.GetAllocator());

        rapidjson::Value statements(rapidjson::kArrayType);
        json.AddMember("statements", m_statement->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Identifier> m_name;
    std::shared_ptr<StatementBlock> m_statement;
    std::vector<std::shared_ptr<Identifier>> m_initial_list;
};

class Program : public Statement // 根节点
{
public:
    Program() : Statement(Type::NODE_PROGRAM) {}
    ~Program() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        for (auto &stat : m_statements)
        {
            statements.PushBack(stat->json(father), father.GetAllocator());
        }
        json.AddMember("statements", statements, father.GetAllocator());

        if (!m_functions.empty())
        {
            rapidjson::Value functions(rapidjson::kArrayType);
            for (auto &fn : m_functions)
            {
                functions.PushBack(fn->json(father), father.GetAllocator());
            }
            json.AddMember("functions", functions, father.GetAllocator());
        }
        return json;
    }

public:
    std::vector<std::shared_ptr<Statement>> m_statements;
    std::vector<std::shared_ptr<Function>> m_functions;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() : Statement(Type::NODE_EXPRESSION_STATEMENT) {}
    ~ExpressionStatement() {}

    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        json.AddMember("expression", m_expression->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Expression> m_expression;
};



class IfStatement : public Statement
{
public:
    IfStatement() : Statement(Node::NODE_IFSTATEMENT) {}
    ~IfStatement() {}
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        json.AddMember("expression", m_expression->json(father), father.GetAllocator());
        json.AddMember("true_statement", m_true_statement->json(father), father.GetAllocator());
        // json.AddMember("false_statement", m_true_statement->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Expression> m_expression;
    std::shared_ptr<Statement> m_true_statement;
    // std::shared_ptr<Statement> m_false_statement;
};

class WhileStatement : public Statement
{
public:
    WhileStatement() : Statement(Node::NODE_WHILESTATEMENT) {}
    ~WhileStatement() {}
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        rapidjson::Value statements(rapidjson::kArrayType);
        json.AddMember("expression", m_expression->json(father), father.GetAllocator());
        json.AddMember("cycle_statement", m_cycle_statement->json(father), father.GetAllocator());
        return json;
    }

public:
    std::shared_ptr<Expression> m_expression;
    std::shared_ptr<Statement> m_cycle_statement;
    // std::shared_ptr<Statement> m_false_statement;
};

class BreakStatement : public Statement
{
public:
    BreakStatement() : Statement(Node::NODE_BREAKSTATEMENT) {}
    ~BreakStatement() {}
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        return json;
    }
};

class ContinueStatement : public Statement
{
public:
    ContinueStatement() : Statement(Node::NODE_CONTINUESTATEMENT) {}
    ~ContinueStatement() {}
    virtual rapidjson::Value json(rapidjson::Document &father)
    {
        rapidjson::Value json(rapidjson::kObjectType);
        std::string *typeStr = new std::string;
        *typeStr = name();
        json.AddMember("type", rapidjson::StringRef(typeStr->c_str()), father.GetAllocator());
        return json;
    }
};
