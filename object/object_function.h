// 为了高价函数预留的对象

#include "object.h"
#include "../ast/node.h"

class Ob_Funtion : public Object
{
public:
    Ob_Funtion(std::shared_ptr<Node> node) : Object(Object::OBJECT_INDEX), m_node(node) {}
    ~Ob_Funtion() {}

    virtual std::shared_ptr<Object> clone()
    {
        return std::make_shared<Ob_Funtion>(*this);
    }
    virtual std::string str() const
    {
        return "";
    }

public:
    std::shared_ptr<Node> m_node;
};
