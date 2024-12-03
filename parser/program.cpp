#include "parser.h" //创建程序的根节点

void Parser::parse_program()
{
    // std::shared_ptr<Program> s(new Program());
    while (m_curr.type != TokenType::EOF_TOKEN && m_curr.type != TokenType::SEMICOLON && m_curr.type != TokenType::RIGHT_BRACE) // 解析程序
    {
        std::shared_ptr<Statement> stmt = parse_statement();
        if (stmt == nullptr) // pass comment
        {
            return;
        }
        if (stmt.get()->m_type != Node::Type::NODE_COMMENT && errors().empty()) // 如果指针有效
        {
            if (stmt->type() == Node::NODE_FUNCTION)
                m_program->m_functions.push_back(std::dynamic_pointer_cast<Function>(stmt));
            else
                m_program->m_statements.push_back(stmt);
        }
        if (m_curr.type != TokenType::SEMICOLON)
            next_token();
    }
    // return s;
}
