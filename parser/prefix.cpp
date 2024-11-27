#include "parser.h"
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
