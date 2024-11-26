#pragma once
#include "lexer.h"

static std::map<std::string, TokenType> keyWords = {
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"xor", TokenType::XOR},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"continue", TokenType::CONTINUE},
    {"break", TokenType::BREAK},
    {"while", TokenType::WHILE},
    {"class", TokenType::CLASS},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"this", TokenType::THIS},
    {"var", TokenType::VAR},
    {"do", TokenType::DO},
    {"EOF", TokenType::EOF_TOKEN},
    {"nil", TokenType::NIL},
    {"ERR", TokenType::ERR},
};

std::vector<Token> Lexer::scanTokens()
{
    char inpt;
    //int equal = 0;
    while (current < length)
    {
        start = current;
        inpt = nextChar();
        scanToken(inpt);
    }
    return tokens;
}

void Lexer::scanToken(char inpt)
{

    switch (inpt)
    {
    case EOF:
        addToken(EOF_TOKEN);
        break;
    case ' ':
        break;
    case '\t':
        break;
    case '\n':
        line++;
        break;
    case '(':
        addToken(LEFT_PAREN);
        break;
    case ')':
        for (int i = 0; i < bracketStatus; i++)
        {
            addToken(RIGHT_PAREN);
        }
        bracketStatus = 0;
        addToken(RIGHT_PAREN);
        break;
    case '{':
        addToken(LEFT_BRACE);
        break;
    case '}':
        addToken(RIGHT_BRACE);
        break;
    case '[':
        addToken(LEFT_BRACKET);
        break;
    case ']':
        addToken(RIGHT_BRACKET);
        break;
    case '"':
        addToken(DOUBLE_QUOTE);
        break;
    case '\'':
        addToken(SINGLE_QUOTE);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        addToken(DOT);
        break;
    case ':':
        addToken(COLON);
        break;
    case ';':
        for (int i = 0; i < bracketStatus; i++)
        {
            addToken(RIGHT_PAREN);
        }
        bracketStatus = 0;
        addToken(SEMICOLON);
        break;
    case '?':
        addToken(QUESTION);
        break;
    case '-':
        addToken(MINUS);
        break;
    case '+':
        addToken(PLUS);
        break;
    case '/':
        if (nextChar() == '/')
        {
            addToken(SLASH_SLASH);
        }
        else
        {
            current--;
            addToken(SLASH);
        }
        break;
    case '\\':
        addToken(BACK_SLASH);
        break;
    case '*':
        addToken(STAR);
        break;
    case '#':
        addToken(HASH);
        break;
    case '@':
        addToken(AT);
        break;
    case '|':
        addToken(PIPE);
        break;
    case '%':
        addToken(PERCENT);
        break;
    case '!':
        if (nextChar() == '=')
        {
            addToken(BANG_EQUAL);
        }
        else
        {
            current--;
            addToken(BANG);
        }
        break;
    case '=':
        if (nextChar() == '=')
        {
            addToken(EQUAL_EQUAL);
        }
        else
        {
            current--;
            addToken(EQUAL);
            addToken(LEFT_PAREN);
            bracketStatus++;
        }
        break;
    case '>':
    {
        char next = nextChar();
        if (next == '=')
        {
            addToken(GREATER_EQUAL);
            break;
        }
        if (next == '>')
        {
            addToken(GREATER_GREATER);
            break;
        }
        current--;
        addToken(GREATER);
        break;
    }
    case '<':
    {
        char next = nextChar();
        if (next == '=')
        {
            addToken(LESS_EQUAL);
            break;
        }
        else if (next == '<')
        {
            addToken(LESS_LESS);
            break;
        }
        current--;
        addToken(LESS);
        break;
    }
    case '^':
        addToken(BIT_XOR);
        break;
    case '&':
        addToken(BIT_AND);
        break;

    default:
        switch (findType(inpt))
        {
        case 1:
            tokens.push_back(tokenLetter(inpt));
            break;
        case 3:
            tokens.push_back(tokenNumber(inpt));
            break;
        default:
            std::cerr << inpt << "[error:unknown_charactor] id=" << findType(inpt) << std::endl;
            tokens.push_back(Token(TokenType::ERR, std::to_string(inpt), std::monostate(), line));
        }
    }
}

Token Lexer::nextToken()
{
    if (read_current >= (int)tokens.size())
    {
        return Token(TokenType::EMPTY, "", std::monostate(), line);
    }
    return tokens[read_current++];
}

void Lexer::addToken(TokenType type)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, std::monostate(), line));
}

inline char Lexer::nextChar()
{
    char inpt = source[current];
    current++;
    return inpt;
}

inline bool Lexer::isEmpty(char inpt)
{
    return (inpt == ' ' || inpt == '\n' || inpt == '\t');
}

inline bool Lexer::isLetter(char inpt)
{
    return (inpt >= 'a' && inpt <= 'z') || (inpt >= 'A' && inpt <= 'Z') || inpt == '_';
}

inline bool Lexer::isNumber(char inpt)
{
    return (inpt >= '0' && inpt <= '9');
}

inline bool Lexer::isRelationalOperator(char inpt)
{
    return (inpt == '=' || inpt == '<' || inpt == '>');
}

inline bool Lexer::isMathematicalOperator(char inpt)
{
    return (inpt == '+' || inpt == '-' || inpt == '*' || inpt == '/' || inpt == '%' || inpt == '^' || inpt == '&');
}

inline bool Lexer::isDelimiter(char inpt)
{
    return (inpt == ';') || (inpt == ',') || (inpt == ',') || (inpt == '[') || (inpt == ']') || (inpt == '{') || (inpt == '}') || (inpt == '(') || (inpt == ')');
}

int Lexer::findType(char inpt)
{
    if (this->isLetter(inpt))
        return 1;
    else if (this->isNumber(inpt))
        return 3;
    else if (this->isMathematicalOperator(inpt))
        return 4;
    else if (this->isRelationalOperator(inpt))
        return 5;
    else if (this->isDelimiter(inpt))
        return 6;
    else if (this->isEmpty(inpt))
        return 0;
    else
        return -1;
}

int Lexer::isKeywords(char *kw)
{
    int type;
    for (type = 0; type < 9; type++)
    {
        if (!strcmp(keywords[type], kw))
        {
            return type;
        }
    }
    return -1;
}

Token Lexer::tokenLetter(char inpt)
{
    std::string letters;
    while (isNumber(inpt) || isLetter(inpt) || inpt == '_')
    {
        letters += inpt;
        inpt = this->nextChar();
    }
    current--;

    auto it = keyWords.find(letters); // 查找输入字符串

    if (it != keyWords.end())
    {
        // 找到对应的键，访问其值
        return Token(it->second, letters, std::monostate(), line);
    }
    else
    {
        // 没有找到
        return Token(TokenType::IDENTIFIER, letters, letters, line);
    }
}
Token Lexer::tokenNumber(char inpt)
{
    std::string digits;
    while (isNumber(inpt))
    {
        digits.push_back(inpt);
        inpt = this->nextChar();
    }
    current--;
    // fseek(file, -1, SEEK_CUR);
    if (digits.size() > 18)
    {
        std::cerr << digits << "[error:out_of_number_MAX(999999999999999999)]" << std::endl;
        return Token(TokenType::ERR, digits, std::monostate(), line);
    }
    return Token(TokenType::INTEGER, digits, std::stoll(digits), line);
}
void Lexer::processLetter(char inpt) // 处理字母
{
    std::string keyword;
    while (isNumber(inpt) || isLetter(inpt) || inpt == '_')
    {
        keyword += inpt;
        inpt = this->nextChar();
    }
    current--;

    auto it = keyWords.find(keyword); // 查找输入字符串

    if (it != keyWords.end())
    {
        // 找到对应的键，访问其值
        std::cout << keyword << "[keywords] id=" << 1 << std::endl;
    }
    else
    {
        // 没有找到
        std::cout << keyword << "[identifier] id=" << 2 << std::endl;
    }
}

void Lexer::processNumber(char inpt) // 处理数字
{
    char digit[100];
    int pd = 0;
    while (isNumber(inpt))
    {
        digit[pd++] = inpt;
        inpt = this->nextChar();
    }
    current--;
    // fseek(file, -1, SEEK_CUR);
    digit[pd] = '\0';
    std::cout << digit << "[value] id=" << 3 << std::endl;
}

int Lexer::processRelationalOperator(char inpt) // 处理关系运算符
{
    char relationop[3];
    relationop[0] = inpt;
    char nextchar = nextChar();
    if (isRelationalOperator(nextchar))
    {
        relationop[1] = nextchar;
    }
    else
        current--;
    relationop[2] = '\0';

    int type;
    for (type = 0; type < 7; type++)
    {
        if (strcmp(relationop, relation_calcu_table[type]))
            break;
    }
    std::cout << relationop << "[relational_operator type " << type << "] id=5" << std::endl;
    return type;
}

int Lexer::processMathematicalOperator(char inpt)
{
    char maticaliop[15];
    int pr = 0;
    while (isMathematicalOperator(inpt))
    {
        maticaliop[pr++] = inpt;
        inpt = nextChar();
    }
    current--;
    // fseek(file, -1, SEEK_CUR);
    maticaliop[pr] = '\0';
    int type;
    for (type = 0; type < 7; type++)
    {
        if (!strcmp(maticaliop, cal_sign[type]))
            break;
    }
    std::cout << maticaliop << "[matical_operator type " << type << "] id=4" << std::endl;
    return type;
}

int Lexer::processDelimiter(char inpt)
{
    char deli[2];
    deli[0] = inpt;
    deli[1] = '\0';
    int type;
    for (type = 0; type < 8; type++)
    {
        if (!strcmp(deli, space_word_table[type]))
            break;
    }
    std::cout << deli << "[delimiter type " << type << "]" << " id=6" << std::endl;
    return type;
}
