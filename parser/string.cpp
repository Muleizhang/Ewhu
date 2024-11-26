#pragma once
#include "parser.h"

std::shared_ptr<Expression> Parser::parse_string()
{
    std::shared_ptr<String> ele(new String());
    ele->m_token = this->m_curr;
    ele->m_value = m_curr.literalToString(); // 转换
    return ele;
}