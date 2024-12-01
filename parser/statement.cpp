#include "parser.h"

std::shared_ptr<Statement> Parser::parse_statement_block()
{
    std::shared_ptr<StatementBlock> ele(new StatementBlock());
    while (m_curr.type != TokenType::EOF_TOKEN && m_peek.type != TokenType::RIGHT_BRACE) // 解析代码块
    {
        next_token();
        std::shared_ptr<Statement> stmt = parse_statement();
        if (stmt == nullptr) // pass comment
        {
            return nullptr;
        }
        if (stmt.get()->m_type != Node::Type::NODE_COMMENT) // 如果指针有效
        {
            ele->m_statements.push_back(stmt);
        }
    }
    next_token();
    return ele;
}

std::shared_ptr<Statement> Parser::parse_statement()
{
    if (m_curr.type == TokenType::HASH)
    {
        return std::make_shared<Comment>();
    }
    auto it = m_control_flow_fns.find(m_curr.type);
    if (it == m_control_flow_fns.end())
    {
        return parse_expression_statement();
    }
    std::shared_ptr<Statement> ele = (this->*(it->second))();
    return ele;
    /*if (m_curr.type == TokenType::RETURN)
    {
        return parse_return_statement();
    }
    else if (m_curr.type == TokenType::IF)
    {
        return parse_if_statement();
    }
    else if (m_curr.type == TokenType::WHILE)
    {
        return parse_while_statement();
    }
    else if (m_curr.type == TokenType::FOR)
    {
        return parse_for_statement();
    }
    else
    {*/

    //}
}

std::shared_ptr<ExpressionStatement> Parser::parse_expression_statement()
{
    std::shared_ptr<ExpressionStatement> s(new ExpressionStatement);
    s->m_token = m_curr;
    s->m_expression = parse_expression(Precedence::LOWEST);
    if (peek_token_is(TokenType::SEMICOLON)) // while->if暂定
    {
        next_token();
    }
    return s;
}

std::shared_ptr<Statement> Parser::parse_while_statement()
{
    std::shared_ptr<WhileStatement> ele(new WhileStatement());
    ele->m_token = m_curr;
    next_token();
    ele->m_expression = parse_expression(LOWEST);
    next_token();
    ele->m_cycle_statement = parse_statement();
    return ele;
}

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

std::shared_ptr<Statement> Parser::parse_break_statement()
{
    std::shared_ptr<BreakStatement> ele(new BreakStatement());
    next_token();
    return ele;
}