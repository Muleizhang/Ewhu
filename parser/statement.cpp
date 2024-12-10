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

std::shared_ptr<Statement> Parser::parse_continue_statement()
{
    std::shared_ptr<ContinueStatement> ele(new ContinueStatement());
    next_token();
    return ele;
}

std::shared_ptr<Statement> Parser::parse_function_declaration()
{
    std::shared_ptr<Function> fn(new Function());
    next_token();
    std::shared_ptr<Identifier> ele(new Identifier());
    ele->m_token = this->m_curr;
    ele->m_name = m_curr.literalToString(); // 转换
    fn->m_func = ele;
    next_token();
    if (m_curr.type == TokenType::LEFT_PAREN)
    {
        while (m_peek.type != TokenType::RIGHT_PAREN)
        {
            next_token();
            auto arg = parse_identifier();
            if (arg->m_token.type != TokenType::IDENTIFIER)
            {
                m_errors.push_back("Parser::Invalid Function Argument List");
                return nullptr;
            }
            fn->m_initial_list.push_back(std::dynamic_pointer_cast<Identifier>(arg));
            if (m_peek.type == TokenType::COMMA)
                next_token();
        }
        next_token();
        next_token();
    }
    else
        ;
    std::shared_ptr<StatementBlock> stmt = std::dynamic_pointer_cast<StatementBlock>(parse_statement_block());
    if (stmt)
        fn->m_statement = stmt;
    next_token();

    // while (m_curr.type != TokenType::SEMICOLON && m_curr.type != TokenType::RIGHT_BRACE)
    // {
    //     std::shared_ptr<Statement> stmt = parse_statement();
    //     if (stmt.get()->m_type != Node::Type::NODE_COMMENT && errors().empty()) // 如果指针有效
    //     {
    //         fn->m_statements.push_back(stmt);
    //     }
    //     if (m_curr.type != TokenType::SEMICOLON)
    //         next_token();
    // }
    return fn;
}

std::shared_ptr<Statement> Parser::parse_return_statement()
{
    std::shared_ptr<ReturnStatement> ele(new ReturnStatement);
    next_token();
    ele->m_expression_statement = parse_expression_statement();
    return ele;
}