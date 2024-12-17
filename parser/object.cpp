#include "parser.h"

std::shared_ptr<Expression> Parser::parse_identifier()
{
    if (m_peek.type == TokenType::LEFT_PAREN)
    {
        return parse_identifier_function();
    }
    auto ele = std::make_shared<Identifier>();
    ele->m_token = this->m_curr;
    auto string_name = m_curr.literalToString();
    ele->m_name = hash(string_name); // 转换
    identifier_map.insert({ele->m_name, string_name});
    return ele;
}

std::shared_ptr<Expression> Parser::parse_identifier_function()
{
    auto ele = std::make_shared<FunctionIdentifier>();
    ele->m_token = this->m_curr;
    auto string_name = m_curr.literalToString();
    ele->m_name = hash(string_name); // 转换
    function_map.insert({ele->m_name, string_name});
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
    auto ele = std::make_shared<Integer>();
    ele->m_token = this->m_curr;
    ele->m_value = m_curr.literalToLonglong(); // 转换
    ele->m_int = std::make_shared<Ob_Integer>(ele->m_value);
    return ele;
}

std::shared_ptr<Expression> Parser::parse_boolean()
{
    auto ele = std::make_shared<Boolean>();
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
