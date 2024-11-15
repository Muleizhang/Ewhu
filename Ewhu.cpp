#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

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
            exit(-1);
        }

        std::string line;
        int lineNum = 1;
        std::vector<Token> token;
        Parser parser;
        int bracketStatus = 0;
        while (std::getline(file, line))
        {
            std::cout << lineNum++ << " ";
            run(line, token, parser, bracketStatus);
            std::cout << std::endl;
        }

        file.close();
    }

    static void runPrompt()
    {
        int lineNum = 1;
        int bracketStatus = 0;
        std::string line;
        std::vector<Token> tokens;
        Parser parser;

        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
            {
                break;
            }
            run(line, tokens, parser, bracketStatus);
        }
    }

    static void run(const std::string &source, std::vector<Token> &tokens,
                    Parser &parser, int &bracketStatus)
    {
        Lexer lx(source);
        std::vector<Token> new_tokens = lx.scanTokens();
        bracketStatus += lx.bracketStatus;
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
        if (bracketStatus == 0) // 前后括号相等
        {
            for (auto token : tokens)
            {
                std::cout << token.toString();
            }
            // parser.parse_program(tokens.begin()); 先不执行
            std::vector<Token> tokens;
            bracketStatus = 0;
        }

        std::cout << std::endl;
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
