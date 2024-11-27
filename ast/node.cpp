#include "node.h"

std::unordered_map<Type, std::string> Node::m_names = {
    {Type::NODE_INTEGER, "Integer"},
    {Type::NODE_FLOAT, "Float"},
    {Type::NODE_STRING, "String"},
    {Type::NODE_BOOLEAN, "Boolean"},
    {Type::NODE_INFIX, "Infix"},
    {Type::NODE_IDENTIFIER, "Identifier"},
    {Type::NODE_EXPRESSION_STATEMENT, "ExpressionStatement"},
    {Type::NODE_PROGRAM, "Program"},
    {Type::NODE_PREFIX, "Prefix"},
    {Type::NODE_STATEMENTBLOCK, "StatementBlock"},
    {Type::NODE_IFSTATEMENT, "IfStatement"},
}; // return the string of the node type
