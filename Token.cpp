#include <string>
#include <variant>
#include <vector>
#include <variant>

// 定义 TokenType 枚举类型，表示标记的类型
enum class TokenType
{
    // Single-character tokens.
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    LEFT_BRACE,  // {
    RIGHT_BRACE, // }
                 // [
                 // ]
    COMMA,       // ,
    DOT,         // .
    MINUS,       // -
    PLUS,        // +
    SEMICOLON,   // ;
    SLASH,       // /
    STAR,        // *

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

    // Keywords.
    AND,       // and
    CLASS,     // class
    ELSE,      // else
    FALSE,     // false
               // FUNCTION, //
    FOR,       // for
    IF,        // if
    NIL,       // NULL
    OR,        // or
    PRINT,     // print
    RETURN,    // return
    THIS,      // this
    TRUE,      // true
    VAR,       // var
    WHILE,     // while
    DO,        // do
    EOF_TOKEN, // end of file
    ERR,       // error
};

// 定义 Token 类
class Token
{
    friend class Lexer;
    friend class Parser;
    friend class Tokens;

public:
    TokenType type;                                            // 标记的类型
    std::string lexeme;                                        // 标记对应的源代码文本
    std::variant<std::monostate, double, std::string> literal; // 字面量的值
    int line;                                                  // 标记所在的行号

    // 构造函数
    Token(TokenType type, const std::string &lexeme, std::variant<std::monostate, double, std::string> literal, int line)
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
        else if (std::holds_alternative<double>(literal))
        {
            literalStr = std::to_string(std::get<double>(literal));
        }
        else if (std::holds_alternative<std::string>(literal))
        {
            literalStr = std::get<std::string>(literal);
        }

        // 将 Token 的信息拼接成字符串
        return std::string("[") + std::to_string(static_cast<int>(type)) + " " + lexeme + " " + literalStr + "]";
    }
};
