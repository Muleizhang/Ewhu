#include "parser.h" //创建程序的根节点
#pragma once
std::shared_ptr<Program>::parse_program()
{
    std::shared_ptr<Program> s(new Program());
    while (m_curr.type != TokenType::EOF_TOKEN) // 解析程序
    {
        auto stmt = parse_statement();
        if (stmt) // 如果指针有效
        {
            s->m_statements.push_back(stmt);
        }
        next_token();
    }
    return s;
}