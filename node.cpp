#include "node.h"
std::map<Type, std::string> m_names = {
    {Type::NODE_INTEGER, "Integer"},
    {Type::NODE_INFIX, "Infix"},
    {Type::NODE_EXPRESSION_STATEMENT, "ExpressionStatement"},
    {Type::NODE_PROGRAM, "Program"},
};

inline string Node::name()
{
    auto it = m_names.find(m_type);
    if (it != m_names.end())
        return *it;
    else
        return "";
}