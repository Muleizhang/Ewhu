#pragma once
#include "parser.h"
std::map<TokenType, int> Parser::m_precedences =
    {
        {TokenType::EQUAL, ASSIGN},
        {TokenType::BIT_XOR, BIT},
        {TokenType::BIT_AND, BIT},

        {TokenType::EQUAL_EQUAL, EQUALS},
        {TokenType::BANG_EQUAL, EQUALS},
        {TokenType::LESS, EQUALS},
        {TokenType::GREATER, EQUALS},
        {TokenType::LESS_EQUAL, EQUALS},
        {TokenType::GREATER_EQUAL, EQUALS},

        {TokenType::MINUS, SUM},
        {TokenType::PLUS, SUM},

        {TokenType::STAR, PRODUCT},
        {TokenType::SLASH, PRODUCT},
        {TokenType::PERCENT, PRODUCT},
        {TokenType::SLASH_SLASH, PRODUCT},

        {TokenType::DOT, DOT},
};
std::map<TokenType, Parser::prefix_parse_fn> Parser::m_prefix_parse_fns =
    {
        {TokenType::TRUE, &Parser::parse_boolean},
        {TokenType::FALSE, &Parser::parse_boolean},
        {TokenType::INTEGER, &Parser::parse_integer},
        {TokenType::STRING, &Parser::parse_string},
        {TokenType::LEFT_PAREN, &Parser::parse_group},
        {TokenType::PLUS, &Parser::parse_prefix},
        {TokenType::MINUS, &Parser::parse_prefix},
        {TokenType::BANG, &Parser::parse_prefix},
        {TokenType::IDENTIFIER, &Parser::parse_identifier},
};
std::map<TokenType, Parser::infix_parse_fn> Parser::m_infix_parse_fns =
    {
        {TokenType::PLUS, &Parser::parse_infix},
        {TokenType::MINUS, &Parser::parse_infix},
        {TokenType::STAR, &Parser::parse_infix},
        {TokenType::SLASH, &Parser::parse_infix},
        {TokenType::SLASH_SLASH, &Parser::parse_infix},
        {TokenType::PERCENT, &Parser::parse_infix},

        {TokenType::EQUAL_EQUAL, &Parser::parse_infix},
        {TokenType::BANG_EQUAL, &Parser::parse_infix},
        {TokenType::LESS, &Parser::parse_infix},
        {TokenType::GREATER, &Parser::parse_infix},
        {TokenType::LESS_EQUAL, &Parser::parse_infix},
        {TokenType::GREATER_EQUAL, &Parser::parse_infix},

        {TokenType::DOT, &Parser::parse_infix},

        {TokenType::EQUAL, &Parser::parse_infix},
        {TokenType::BIT_XOR, &Parser::parse_infix},
        {TokenType::BIT_AND, &Parser::parse_infix},

};
std::map<TokenType, Parser::control_flow_fn> Parser::m_control_flow_fns =
    {
        {TokenType::LEFT_BRACE, &Parser::parse_statement_block},
        {TokenType::IF, &Parser::parse_if_statement},
};

void Parser::next_token() // 读取下一个token
{
    m_curr = m_peek;
    if (m_ptokens != m_ptokens_end)
    {
        m_peek = *m_ptokens;
        m_ptokens++;
    }
}

Parser::Parser() // 默认构造函数
{
    m_program.reset(new Program());
}

Parser::Parser(std::vector<Token>::iterator ptokens)
{
    m_program.reset(new Program());
    m_ptokens = ptokens;
    next_token();
    next_token();
}

Parser::~Parser() {}

bool Parser::curr_token_is(TokenType ty)
{
    return m_curr.type == ty;
}

bool Parser::peek_token_is(TokenType ty)
{
    return m_peek.type == ty;
}

bool Parser::expect_peek_token(TokenType ty)
{
    if (peek_token_is(ty))
    {
        next_token();
        return true;
    }
    else
    {
        peek_error(ty);
        return false;
    }
}

void Parser::peek_error(TokenType type)
{
    std::ostringstream oss;
    oss << "Parser: expected next token to be " << TokenTypeToString[type] << ", got " << TokenTypeToString[m_peek.type] << " instead";
    m_errors.push_back(oss.str());
}

int Parser::curr_token_precedence()
{
    auto it = m_precedences.find(m_curr.type);
    if (it != m_precedences.end())
        return it->second;
    else
        return LOWEST;
}

int Parser::peek_token_precedence()
{
    auto it = m_precedences.find(m_peek.type);
    if (it != m_precedences.end())
        return it->second;
    else
        return LOWEST;
}

void Parser::no_prefix_parse_fn_error(TokenType type)
{
    std::ostringstream oss;
    oss << "Parser: no prefix function for " << TokenTypeToString[type] << " found";
    m_errors.push_back(oss.str());
}

std::list<std::string> &Parser::errors()
{
    return m_errors;
}

void Parser::new_sentence(std::vector<Token>::iterator ptokens_start, std::vector<Token>::iterator ptokens_end)
{
    m_ptokens = ptokens_start;
    m_ptokens_end = ptokens_end;
    next_token();
    next_token();
}
