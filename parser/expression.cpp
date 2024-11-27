#include "parser.h"

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