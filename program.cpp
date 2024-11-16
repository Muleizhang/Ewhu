#pragma once
#include "parser.h" //创建程序的根节点
#include "statement.cpp"
void Parser::parse_program()
{
    // std::shared_ptr<Program> s(new Program());
    while (m_curr.type != TokenType::EOF_TOKEN && m_curr.type != TokenType::SEMICOLON) // 解析程序
    {
        auto stmt = parse_statement();
        if (stmt) // 如果指针有效
        {
            m_program->m_statements.push_back(stmt);
        }
        next_token();
    }
    // return s;
}
