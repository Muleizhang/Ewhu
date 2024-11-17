#include <iostream>
#include "lexer.cpp"
#include "parser.cpp"
#include "head.h"


int main()
{
    std::shared_ptr<Lexer> lexer(new Lexer);
    std:shared_ptr<Parser> parser(new Parser);
}