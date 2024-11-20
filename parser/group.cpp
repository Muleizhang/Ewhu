// 运算式中()优先级较高，成为一个group
#pragma once
#include "parser.h"

std::shared_ptr<Expression> Parser::parse_group()
{
    next_token();
    auto ele = parse_expression(LOWEST);
    if (!expect_peek_token(TokenType::RIGHT_PAREN))
    {
        return nullptr;
    }
    return ele;
}