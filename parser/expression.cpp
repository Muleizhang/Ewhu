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

std::shared_ptr<Expression> Parser::parse_expression(int precedence)
{
    auto prefix = m_prefix_parse_fns.find(m_curr.type);
    if (prefix == m_prefix_parse_fns.end())
    {
        no_prefix_parse_fn_error(m_curr.type);
        return nullptr;
    }
    std::shared_ptr<Expression> ele = (this->*(prefix->second))();
    while (!peek_token_is(TokenType::SEMICOLON) && precedence < peek_token_precedence())
    {
        auto infix = m_infix_parse_fns.find(m_peek.type);
        if (infix == m_infix_parse_fns.end())
        {
            return ele;
        }
        next_token();
        ele = (this->*(infix->second))(ele);
    }
    return ele;
}

std::shared_ptr<Expression> Parser::parse_prefix()
{
    std::shared_ptr<Prefix> ele(new Prefix());
    ele->m_token = m_curr;
    ele->m_operator = m_curr.type;
    // int precedence = curr_token_precedence();
    next_token();
    ele->m_right = parse_expression(PRE_SIGN);
    return ele;
}

std::shared_ptr<Expression> Parser::parse_infix(const std::shared_ptr<Expression> &left)
{
    std::shared_ptr<Infix> ele(new Infix());
    ele->m_token = m_curr;
    ele->m_operator = m_curr.type;
    ele->m_left = left;
    int precedence = curr_token_precedence();
    next_token();
    ele->m_right = parse_expression(precedence);
    return ele;
}
