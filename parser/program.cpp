#pragma once
#include "parser.h" //创建程序的根节点
#include "statement.cpp"
void Parser::parse_program()
{
    // std::shared_ptr<Program> s(new Program());
    while (m_curr.type != TokenType::EOF_TOKEN && m_curr.type != TokenType::SEMICOLON) // 解析程序
    {
        std::shared_ptr<Statement> stmt = parse_statement();
        if (stmt.get()->m_type == Node::Type::NODE_COMMENT) // pass comment
        {
            return;
        }
        if (stmt) // 如果指针有效
        {
            // std::cout << stmt << std::endl;
            // std::cout << m_program << std::endl;
            m_program->m_statements.push_back(stmt);
        }
        if (m_curr.type != TokenType::SEMICOLON)
            next_token();
    }
    // return s;
}
