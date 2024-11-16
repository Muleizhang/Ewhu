#include <iostream>
#include <fstream>
#include <string>
#include "lexer.cpp"
#include "parser.cpp"
#include "program.cpp"
#include "integer.cpp"
#include "group.cpp"
#include "infix.cpp"
#include "expression.cpp"

class Ewhu
{
private:
public:
    static void runFile(const std::string &path)
    {
        // 打开文件并读取内容
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << path << std::endl;
            exit(-1); // 文件打开失败，退出程序
        }

        std::string line;
        int lineNum = 1;
        while (std::getline(file, line))
        {
            std::cout << lineNum++ << " ";
            run(line);
            std::cout << std::endl;
        }

        file.close();
    }

    static void runPrompt()
    {
        std::string line;

        while (true)
        {
            std::cout << "> ";
            if (!std::getline(std::cin, line))
            {
                break;
            }

            run(line);
        }
    }

    static void run(const std::string &source)
    {
        Parser parser;
        // 解释 Ewhu 代码的逻辑
        // std::cout << "Running: " << source << std::endl;
        Lexer lx(source);
        std::vector<Token> tokens;
        tokens = lx.scanTokens();

        for (auto token : tokens)
        {
            std::cout << token.toString();
        }
        std::cout << std::endl;

        // 前后括号相等
        if (tokens.end()->type == TokenType::SEMICOLON)
        {
            parser.new_sentence(tokens.begin());
            parser.parse_program();
        }
    }
};

int main(int argc, char *argv[])
{
    // SetConsoleOutputCP(CP_UTF8);

    if (argc > 2)
    {
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;

        exit(64);
    }
    else if (argc == 2)
    {
        Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    }
    else
    {
        Ewhu::runPrompt(); // 进入交互模式
    }

    return 0;
}
