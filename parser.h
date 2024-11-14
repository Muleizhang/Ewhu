#pragma once

#include <memory>
#include "token.h"

class Parser
{
public:
    enum Precedence
    {
        LOWEST,
        LOGICAL,     // and or xor
        EQUALS,      // == !=
        LESSGREATER, // > >= <= <
        SUM,         // + -
        PRODUCT,     // * /
        POWER,       // ^
        PREFIX,      // -X !X
        CALL,        // myFunction(X)
    };
    std::shared_ptr<std::vector<Token>> tokens;
    Token curToken;
    Token peekToken;

public:
    Parser() {};
    ~Parser() {};
};