#include "parser.h"

std::shared_ptr<Expression> Parser::parse_identifier()
{
    std::shared_ptr<Identifier> ele(new Identifier());
    ele->m_token = this->m_curr;
    ele->m_name = m_curr.literalToString(); // 转换
    return ele;
}