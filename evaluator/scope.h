#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include "../object/object.h"
#include "../ast/statement.h"

class Scope
{
public:
    Scope(std::unordered_map<std::string, std::shared_ptr<Object>> scp,
          std::unordered_map<std::string, std::shared_ptr<Node>> func)
        : m_var(scp), m_func(func) {}
    Scope(Scope *father) : father(father) {}

    Scope() {}
    ~Scope()
    {
        for (auto it : m_var)
        {
            it.second.reset();
        }
        m_var.clear();
    };
    void print()
    {
        for (const auto &var : m_var)
        {
            std::cout << "Variable: " << var.first << std::endl;
        }

        for (const auto &func : m_func)
        {
            std::cout << "Function: " << func.first << std::endl;
        }
    }

public:
    Scope *father = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Object>> m_var;
    std::unordered_map<std::string, std::shared_ptr<Node>> m_func;
};
