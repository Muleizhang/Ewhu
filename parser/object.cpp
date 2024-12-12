#include "parser.h"

std::shared_ptr<Expression> Parser::parse_identifier()
{
    if (m_peek.type == TokenType::LEFT_PAREN)
    {
        return parse_identifier_function();
    }
    std::shared_ptr<Identifier> ele(new Identifier());
    ele->m_token = this->m_curr;
    ele->m_name = hash_string_to_int(m_curr.literalToString()); // 转换
    return ele;
}

std::shared_ptr<Expression> Parser::parse_identifier_function()
{
    std::shared_ptr<FunctionIdentifier> ele = std::make_shared<FunctionIdentifier>();
    ele->m_token = this->m_curr;
    ele->m_name = hash_string_to_int(m_curr.literalToString()); // 转换
    next_token();
    if (m_curr.type == TokenType::LEFT_PAREN)
    {
        while (m_peek.type != TokenType::RIGHT_PAREN)
        {
            next_token();
            auto arg = parse_expression(Precedence::LOWEST);
            ele->m_initial_list.push_back(std::dynamic_pointer_cast<Expression>(arg));
            if (m_peek.type == TokenType::COMMA)
                next_token();
        }
        next_token();
    }
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
    ele->m_bool = (m_curr.type == TokenType::TRUE) ? true : false;
    return ele;
}

std::shared_ptr<Expression> Parser::parse_string()
{
    std::shared_ptr<String> ele(new String());
    ele->m_token = this->m_curr;
    ele->m_string = m_curr.literalToString(); // 转换
    return ele;
}
