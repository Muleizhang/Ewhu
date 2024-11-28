#pragma once
#include "parser.h"
std::shared_ptr<Statement> Parser::parse_while_statement()
{
    std::shared_ptr<WhileStatement> ele(new WhileStatement());
    ele->m_token = m_curr;
    next_token();
    ele->m_expression = parse_expression(LOWEST);
    next_token();
    ele->m_cycle_statement = parse_statement();
    return ele;
}