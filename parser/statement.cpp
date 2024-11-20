// #pragma once
#include "parser.h"
#include "expression.cpp"

std::shared_ptr<Statement> Parser::parse_statement()
{
    return parse_expression_statement();
}

std::shared_ptr<ExpressionStatement> Parser::parse_expression_statement()
{
    std::shared_ptr<ExpressionStatement> s(new ExpressionStatement);
    s->m_token = m_curr;
    s->m_expression = parse_expression(Precedence::LOWEST);
    if (peek_token_is(TokenType::SEMICOLON)) // while->if暂定
    {
        next_token();
    }
    return s;
}