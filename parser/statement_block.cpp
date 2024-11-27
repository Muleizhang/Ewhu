// 运算式中()优先级较高，成为一个group
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
        if (stmt.get()->m_type != Type::NODE_COMMENT) // 如果指针有效
        {
            ele->m_statements.push_back(stmt);
        }
        if (m_curr.type != TokenType::SEMICOLON)
            next_token();
    }
    return ele;
}
