#include "Node.h"

std::map<Node::Type, std::string> Node::TypeToString = {
    {Node::NODE_INTEGER, "Integer"},
    {Node::NODE_FLOAT, "Float"},
    {Node::NODE_STRING, "String"},
    {Node::NODE_BINARY, "Binary"},
    {Node::NODE_INFIX, "Infix"},
    {Node::NODE_IDENTIFIER, "Identifier"},
    {Node::NODE_EXPRESSION_STATEMENT, "ExpressionStatement"},
    {Node::NODE_PROGRAM, "Program"},
};

// return the string of the node type
std::string Node::nameString()
{
    auto it = TypeToString.find(type);
    if (it == TypeToString.end())
    {
        return "Unknown";
    }
    return it->second;
}