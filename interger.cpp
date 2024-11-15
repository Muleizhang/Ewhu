#include "parser.h"
#pragma once
std::shared_ptr<Expression> Parser::parse_integer()
{
    std::shared_ptr<Integer> ele(new Integer());
    ele->m_token = this->m_curr;
    ele->m_value = std::atol(m_curr.literal().c_str()); // 转换
    return e; 
}