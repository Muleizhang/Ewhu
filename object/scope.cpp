// CHATPGT: 作用域类

#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

class Scope
{
public:
    using Value = std::shared_ptr<void>;

    Scope(std::shared_ptr<Scope> parent = nullptr) : parent(parent) {}

    template <typename T>
    void set(const std::string &name, std::shared_ptr<T> value)
    {
        variables[name] = std::static_pointer_cast<void>(value);
    }

    template <typename T>
    std::shared_ptr<T> get(const std::string &name)
    {
        if (variables.find(name) != variables.end())
        {
            return std::static_pointer_cast<T>(variables[name]);
        }
        else if (parent != nullptr)
        {
            return parent->get<T>(name);
        }
        else
        {
            throw std::runtime_error("Variable '" + name + "' not found");
        }
    }

    void remove(const std::string &name)
    {
        if (variables.find(name) != variables.end())
        {
            variables.erase(name);
        }
        else if (parent != nullptr)
        {
            parent->remove(name);
        }
        else
        {
            throw std::runtime_error("Variable '" + name + "' not found");
        }
    }

private:
    std::unordered_map<std::string, Value> variables;
    std::shared_ptr<Scope> parent;
};

/* 示例用法
int main()
{
    // 全局作用域
    auto globalScope = std::make_shared<Scope>();

    // 定义函数 foo
    auto foo = [&]()
    {
        // 创建局部作用域，父作用域为全局作用域
        auto localScope = std::make_shared<Scope>(globalScope);
        localScope->set("x", std::make_shared<int>(10));
        localScope->set("y", std::make_shared<float>(3.14f));
        localScope->set("z", std::make_shared<std::string>("Hello"));

        // 获取并打印变量
        std::cout << *localScope->get<int>("x") << std::endl;
        std::cout << *localScope->get<float>("y") << std::endl;
        std::cout << *localScope->get<std::string>("z") << std::endl;

        // 删除变量
        localScope->remove("x");

        // 尝试获取已删除的变量
        try
        {
            std::cout << *localScope->get<int>("x") << std::endl;
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
        }
    };

    // 调用 foo
    foo();

    return 0;
}
*/
