#pragma once

// 运算式中()优先级较高，成为一个group
#include "parser.h"

std::shared_ptr<Statement> Parser::parse_statement_block()
{
    next_token();
    auto ele = parse_expression(LOWEST);
    if (!expect_peek_token(TokenType::RIGHT_PAREN))
    {
        return nullptr;
    }
    return ele;
}