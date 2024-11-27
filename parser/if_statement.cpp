#include "parser.h"
std::shared_ptr<Statement> Parser::parse_if_statement()
{
    std::shared_ptr<IfStatement> ele(new IfStatement());
    ele->m_token = m_curr;
    next_token();
    ele->m_expression = parse_expression(LOWEST);
    next_token();
    ele->m_true_statement = parse_statement();
    return ele;
}