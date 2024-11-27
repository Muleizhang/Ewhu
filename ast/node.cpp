#pragma once
#include "Node.h"

std::map<Node::Type, std::string> Node::m_names = {
    {Node::NODE_INTEGER, "Integer"},
    {Node::NODE_FLOAT, "Float"},
    {Node::NODE_STRING, "String"},
    {Node::NODE_BOOLEAN, "Boolean"},
    {Node::NODE_INFIX, "Infix"},
    {Node::NODE_IDENTIFIER, "Identifier"},
    {Node::NODE_EXPRESSION_STATEMENT, "ExpressionStatement"},
    {Node::NODE_PROGRAM, "Program"},
    {Node::NODE_PREFIX, "Prefix"},
    {Node::NODE_STATEMENTBLOCK, "StatementBlock"},
    {Node::NODE_IFSTATEMENT, "IfStatement"},
};

// return the string of the node type
std::string Node::name() const
{
    auto it = m_names.find(m_type);
    if (it == m_names.end())
    {
        return "";
    }
    return it->second;
}