// 负责局部变量的作用范围
#include <unordered_map>
#include<map>
#include <string>
#include <memory>
#include "../object/object.h"
class Scope
{
public:
    Scope(){};
    ~Scope(){};

public:
    std::unordered_map<std::string, std::shared_ptr<Object>> m_var;
};