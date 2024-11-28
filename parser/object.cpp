#include "parser.h"

std::shared_ptr<Expression> Parser::parse_identifier()
{
    std::shared_ptr<Identifier> ele(new Identifier());
    ele->m_token = this->m_curr;
    ele->m_name = m_curr.literalToString(); // 转换
    return ele;
}

std::shared_ptr<Expression> Parser::parse_integer()
{
    std::shared_ptr<Integer> ele(new Integer());
    ele->m_token = this->m_curr;
    ele->m_value = m_curr.literalToLonglong(); // 转换
    return ele;
}

std::shared_ptr<Expression> Parser::parse_boolean()
{
    std::shared_ptr<Boolean> ele(new Boolean());
    ele->m_token = this->m_curr;
    ele->m_value = (m_curr.type == TokenType::TRUE) ? true : false;
    return ele;
}

std::shared_ptr<Expression> Parser::parse_string()
{
    std::shared_ptr<String> ele(new String());
    ele->m_token = this->m_curr;
    ele->m_value = m_curr.literalToString(); // 转换
    return ele;
}
