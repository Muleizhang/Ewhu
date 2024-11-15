#include "parser.h"
std::map<TokenType, int> Parser::m_precedences =
    {
        {TokenType::PLUS, SUM},
        {TokenType::MINUS, SUM},
        {TokenType::STAR, PRODUCT},
        {TokenType::SLASH, PRODUCT},
};
std::map<TokenType, Parser::prefix_parse_fn> Parser::m_prefix_parse_fns =
    {
        {TokenType::INTEGER, &Parser::parse_integer},
        {TokenType::LEFT_PAREN, &Parser::parse_group},
};
std::map<TokenType, Parser::infix_parse_fn> Parser::m_infix_parse_fns =
    {
        {TokenType::PLUS, &Parser::parse_infix},
        {TokenType::MINUS, &Parser::parse_infix},
        {TokenType::STAR, &Parser::parse_infix},
        {TokenType::SLASH, &Parser::parse_infix},
};

void Parser::next_token() // 读取下一个token
{
    m_curr = m_peek;
    m_peek = *m_ptokens;
    m_ptokens++;
}

Parser::Parser() // 默认构造函数
{
}

Parser::Parser(std::vector<Token>::iterator ptokens)
{
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
    oss << "expected next token to be " << type << ", got" << m_peek.type << " instead";
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
    oss << "no prefix function for " << type;
    m_errors.push_back(oss.str());
}

std::list<std::string> &Parser::errors()
{
    return m_errors;
}