#include <string>
#include <variant>
#include <vector>
#include <variant>
#include <map>
#include <iomanip>
#pragma once
enum TokenType
{
    // Single-character tokens.
    LEFT_PAREN,    // (
    RIGHT_PAREN,   // )
    LEFT_BRACE,    // {
    RIGHT_BRACE,   // }
    LEFT_BRACKET,  // [
    RIGHT_BRACKET, // ]
    DOUBLE_QUOTE,  // "
    SINGLE_QUOTE,  // '
    COMMA,         // ,
    DOT,           // .
    COLON,         // :
    SEMICOLON,     // ;
    QUESTION,      // ?
    MINUS,         // -
    PLUS,          // +
    SLASH,         // /
    BACK_SLASH,    // '\'
    STAR,          // *
    HASH,          // #
    AT,            // @
    AMPERSAND,     // &
    PIPE,          // |
    PERCENT,       // %

    // One or two character tokens.
    BANG,          // !
    BANG_EQUAL,    //!=
    EQUAL,         // =
    EQUAL_EQUAL,   //==
    GREATER,       //>
    GREATER_EQUAL, //>=
    LESS,          //<
    LESS_EQUAL,    //<=

    // Literals.
    IDENTIFIER, // a
    STRING,     //"a"
    NUMBER,     // 1
    INTEGER,

    // Keywords.
    AND,       // and
    OR,        // or
    XOR,       // xor
    IF,        // if
    ELSE,      // else
    TRUE,      // true
    FALSE,     // false
    FOR,       // for
    CONTINUE,  // continue
    BREAK,     // break
    WHILE,     // while
    CLASS,     // class
    PRINT,     // print
    RETURN,    // return
    THIS,      // this
    VAR,       // var
    DO,        // do
    EOF_TOKEN, // end of file
    NIL,       // NIL
    ERR,       // error
};

static std::map<TokenType, std::string> TokenTypeToString = {
    // Single-character tokens.
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::LEFT_BRACKET, "LEFT_BRACKET"},
    {TokenType::RIGHT_BRACKET, "RIGHT_BRACKET"},
    {TokenType::DOUBLE_QUOTE, "DOUBLE_QUOTE"},
    {TokenType::SINGLE_QUOTE, "SINGLE_QUOTE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::COLON, "COLON"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::QUESTION, "QUESTION"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::BACK_SLASH, "BACK_SLASH"},
    {TokenType::STAR, "STAR"},
    {TokenType::HASH, "HASH"},
    {TokenType::AT, "AT"},
    {TokenType::AMPERSAND, "AMPERSAND"},
    {TokenType::PIPE, "PIPE"},
    {TokenType::PERCENT, "PERCENT"},

    // One or two character tokens.
    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},

    // Literals.
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},

    // Keywords.
    {TokenType::AND, "AND"},
    {TokenType::OR, "OR"},
    {TokenType::XOR, "XOR"},
    {TokenType::IF, "IF"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::FOR, "FOR"},
    {TokenType::CONTINUE, "CONTINUE"},
    {TokenType::BREAK, "BREAK"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::THIS, "THIS"},
    {TokenType::VAR, "VAR"},
    {TokenType::DO, "DO"},
    {TokenType::EOF_TOKEN, "EOF"},
    {TokenType::NIL, "NIL"},
    {TokenType::ERR, "ERR"},
};

// 定义 TokenType 枚举类型，表示标记的类型

// 定义 Token 类
class Token
{
    friend class Lexer;
    friend class Parser;
    friend class Tokens;

public:
    TokenType type;                                         // 标记的类型
    std::string lexeme;                                     // 标记对应的源代码文本
    std::variant<std::monostate, int, std::string> literal; // 字面量的值
    int line;                                               // 标记所在的行号

    // 构造函数
    Token(TokenType type, const std::string &lexeme, std::variant<std::monostate, int, std::string> literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}

    Token() : type(TokenType::ERR), lexeme(""), literal(std::monostate()), line(0) {}
    // 返回 Token 的字符串表示
    std::string toString() const
    {
        std::string literalStr;

        // 处理 literal 的值
        if (std::holds_alternative<std::monostate>(literal))
        {
            literalStr = "null";
        }
        else if (std::holds_alternative<int>(literal))
        {
            literalStr = std::to_string(std::get<int>(literal));
        }
        else if (std::holds_alternative<std::string>(literal))
        {
            literalStr = std::get<std::string>(literal);
        }

        // 将 Token 的信息拼接成字符串
        return std::string("[") + TokenTypeToString[type] + " " + lexeme + " " + literalStr + "]";
    }
};
