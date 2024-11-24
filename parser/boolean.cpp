#pragma once
#include "parser.h"

std::shared_ptr<Expression> Parser::parse_boolean()
{
    std::shared_ptr<Boolean> ele(new Boolean());
    ele->m_token = this->m_curr;
    ele->m_value = (m_curr.type == 40) ? true : false;
    return ele;
}