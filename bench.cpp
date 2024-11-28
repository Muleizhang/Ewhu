#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "lexer/lexer.cpp"
#include "parser/parser.cpp"
#include "parser/head.h"
#include "evaluator/head.h"

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
class Ewhu
{
private:
public:
    static void runFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << path << std::endl;
            exit(-1);
        }

        std::string line;
        int lineNum = 1;
        std::vector<Token> token;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        auto start = std::chrono::high_resolution_clock::now();
        while (std::getline(file, line))
        {
            benchLine(line, token, lexer, parser, evaluator);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "total run time: " << duration.count() << " s" << std::endl
                  << std::endl;

        file.close();
    }

    static void runPrompt()
    {
        int lineNum = 1;
        std::string line;
        std::vector<Token> tokens;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;
            benchLine(line, tokens, lexer, parser, evaluator);
        }
    }
    static void benchLine(const std::string &source, std::vector<Token> &tokens, Lexer &Lexer,
                          Parser &parser, Evaluator &evaluator)
    {
        auto lx_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; i++)
        {
            Lexer.source = source;
            std::vector<Token> new_tokens = Lexer.scanTokens();
            tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
            Lexer.tokens.clear();
        }
        auto lx_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = lx_end - lx_start;
        std::cout << "lx time: " << duration.count() << " s" << std::endl;

        if ((--tokens.end())->type == TokenType::SEMICOLON)
        {
            auto parse_start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 10000; i++)
            {
                parser.new_sentence(tokens.begin(), tokens.end());
                parser.parse_program();
            }
            tokens.clear();
            auto parse_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> parse_duration = parse_end - parse_start;
            std::cout << "parse time: " << parse_duration.count() << " s" << std::endl;

            auto eval_start = std::chrono::high_resolution_clock::now();
            static Scope global_scp;
            for (int i = 0; i < 10000; i++)
            {
                auto program = parser.m_program;
                auto evaluated = evaluator.eval(program, global_scp);
            }
            auto eval_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> eval_duration = eval_end - eval_start;
            std::cout << "evaluate time: " << eval_duration.count() << " s" << std::endl;
        }
    }
};

int main(int argc, char *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cout << "Ewhu Bench Mark" << std::endl;
    if (argc > 1)
    {
        Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    }
    else
    {
        Ewhu::runPrompt(); // 进入交互模式
    }
    return 0;
}
