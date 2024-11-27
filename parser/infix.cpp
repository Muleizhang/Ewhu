#include "parser.h"

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