// 负责局部变量的作用范围
#pragma once
#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include "../object/object.h"

class Scope
{
public:
    Scope(std::unordered_map<std::string, std::shared_ptr<Object>>scp):m_var(scp){}
    Scope(){}
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
};