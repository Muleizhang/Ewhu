#include "ClassLexer.cpp"

inline char Lexer::read()
{
    char inpt = fgetc(file);
    return inpt;
}

inline bool Lexer::isEmpty(char inpt)
{
    return (inpt == ' ' || inpt == '\n' || inpt == '\t');
}

inline bool Lexer::isLetter(char inpt)
{
    return (inpt >= 'a' && inpt <= 'z') || (inpt >= 'A' && inpt <= 'Z');
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
    return (inpt == '+' || inpt == '-' || inpt == '*' || inpt == '/' || inpt == '%');
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

void Lexer::processLetter(char inpt) // 处理字母
{
    char kw[20];
    int pk = 0;
    while (isNumber(inpt) || isLetter(inpt) || inpt == '_')
    {
        kw[pk++] = inpt;
        inpt = this->read();
    }
    fseek(file, -1, SEEK_CUR);
    kw[pk] = '\0';
    if (isKeywords(kw) == -1)
    {
        std::cout << kw << "[identifier] id=" << 2 << std::endl;
    }
    else
    {
        std::cout << kw << "[keywords, type " << isKeywords(kw) << "] id=" << 1 << std::endl;
    }
}

void Lexer::processNumber(char inpt) // 处理数字
{
    char digit[100];
    int pd = 0;
    while (isNumber(inpt))
    {
        digit[pd++] = inpt;
        inpt = this->read();
    }
    fseek(file, -1, SEEK_CUR);
    digit[pd] = '\0';
    std::cout << digit << "[value] id=" << 3 << std::endl;
}

int Lexer::processRelationalOperator(char inpt) // 处理关系运算符
{
    char relationop[3];
    relationop[0] = inpt;
    char nextchar = this->read();
    if (isRelationalOperator(nextchar))
    {
        relationop[1] = nextchar;
    }
    else
        fseek(file, -1, SEEK_CUR);
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
        inpt = this->read();
    }
    fseek(file, -1, SEEK_CUR);
    maticaliop[pr] = '\0';
    int type;
    for (type = 0; type < 5; type++)
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
