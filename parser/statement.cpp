// #pragma once
#include "parser.h"
#include "expression.cpp"

std::shared_ptr<Statement> Parser::parse_statement()
{
    if (m_curr.type == TokenType::HASH)
    {
        return std::make_shared<Comment>();
    }
    auto it = m_control_flow_fns.find(m_curr.type);
    if (it == m_control_flow_fns.end())
    {
        return parse_expression_statement();
    }
    std::shared_ptr<Statement> ele(new)
    /*if (m_curr.type == TokenType::RETURN)
    {
        return parse_return_statement();
    }
    else if (m_curr.type == TokenType::IF)
    {
        return parse_if_statement();
    }
    else if (m_curr.type == TokenType::WHILE)
    {
        return parse_while_statement();
    }
    else if (m_curr.type == TokenType::FOR)
    {
        return parse_for_statement();
    }
    else
    {*/

    //}
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