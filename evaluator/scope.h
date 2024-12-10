// 负责局部变量的作用范围
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "../object/object.h"
#include "../ast/statement.h"

class Scope
{
public:
    Scope(std::unordered_map<std::string, std::shared_ptr<Object>> scp,
          std::unordered_map<std::string, std::shared_ptr<Function>> func)
        : m_var(scp), m_func(func) {}

    Scope() {}
    ~Scope()
    {
        for (auto it : m_var)
        {
            it.second.reset();
        }
        m_var.clear();
    };

public:
    std::unordered_map<std::string, std::shared_ptr<Object>> m_var;
    std::unordered_map<std::string, std::shared_ptr<Function>> m_func;
};
