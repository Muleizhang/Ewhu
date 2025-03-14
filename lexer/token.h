#pragma once
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <iomanip>

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
    PLUS_PLUS,     // ++
    SLASH,         // /
    SLASH_SLASH,   // //
    BACK_SLASH,    // '\'
    STAR,          // *
    STAR_STAR,     // **
    HASH,          // #
    AT,            // @

    // Trignometry
    SIN,     // sin
    COS,     // cos
    TAN,     // tan
             // bitwise operator
    BIT_OR,  // |
    PERCENT, // %
    BIT_XOR, // ^
    BIT_AND, // &

    // One or two character tokens.
    BANG,          // !
    BANG_EQUAL,    //!=
    EQUAL,         // =
    EQUAL_EQUAL,   //==
    GREATER,       //>
    GREATER_EQUAL, //>=
    LESS,          //<
    LESS_EQUAL,    //<=
    SHL,           //<<
    SHR,           //>>

    // Literals.
    IDENTIFIER, // a
    STRING,     //"a"
    INTEGER,    // 1

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
               // PRINT,     // print
    RETURN,    // return
    THIS,      // this
    VAR,       // var
    DO,        // do
    EOF_TOKEN, // end of file
    NIL,       // NIL
    FUNC,      // function
               //  Error
    ERR,       // error

    // Empty
    EMPTY // empty
};

static std::unordered_map<TokenType, std::string> TokenTypeToString = {
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
    {TokenType::PLUS_PLUS, "PLUSPLUS"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::SLASH_SLASH, "SLASH_SLASH"},
    {TokenType::BACK_SLASH, "BACK_SLASH"},
    {TokenType::STAR, "STAR"},
    {TokenType::HASH, "HASH"},
    {TokenType::AT, "AT"},
    {TokenType::PERCENT, "PERCENT"},
    {TokenType::BIT_OR, "BIT_OR"},
    {TokenType::BIT_XOR, "BIT_XOR"},
    {TokenType::BIT_AND, "BIT_AND"},

    // One or two character tokens.
    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::SHR, "SHR"},
    {TokenType::SHL, "SHL"},

    // Literals.
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::INTEGER, "INTEGER"},

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
    //{TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::THIS, "THIS"},
    {TokenType::VAR, "VAR"},
    {TokenType::DO, "DO"},
    {TokenType::EOF_TOKEN, "EOF"},
    {TokenType::NIL, "NIL"},
    {TokenType::FUNC, "FUNC"},
    // Error
    {TokenType::ERR, "ERR"},

    // Empty
    {TokenType::EMPTY, "EMPTY"}};

// 定义 TokenType 枚举类型，表示标记的类型

// 定义 Token 类
class Token
{
    friend class Lexer;
    friend class Parser;
    friend class Tokens;

public:
    TokenType type;                                               // 标记的类型
    std::variant<std::monostate, long long, std::string> literal; // 字面量的值
    int line;                                                     // 标记所在的行号

    // 构造函数
    Token(TokenType type, std::variant<std::monostate, long long, std::string> literal, int line)
        : type(type), literal(literal), line(line) {}

    Token() : type(TokenType::ERR), literal(std::monostate()), line(0) {}

    // 返回 Token 的字符串表示
    std::string toString() const
    {
        std::string literalStr;

        // 处理 literal 的值
        if (std::holds_alternative<std::monostate>(literal))
        {
            literalStr = "null";
        }
        else if (std::holds_alternative<long long>(literal))
        {
            literalStr = std::to_string(std::get<long long>(literal));
        }
        else if (std::holds_alternative<std::string>(literal))
        {
            literalStr = std::get<std::string>(literal);
        }

        // 将 Token 的信息拼接成字符串
        return "[" + TokenTypeToString[type] + " " + literalStr + "]";
    }

    long long literalToLonglong()
    {
        if (std::holds_alternative<long long>(literal))
        {
            return std::get<long long>(literal);
        }
        return 0;
    }

    std::string literalToString()
    {
        if (std::holds_alternative<std::string>(literal))
        {
            return std::get<std::string>(literal);
        }
        return "";
    }
};
