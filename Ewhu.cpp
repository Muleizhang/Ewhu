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
    static void compileFile(const std::string &path, const std::string &output) {}

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
        Parser parser;
        Evaluator evaluator;

        auto start = std::chrono::high_resolution_clock::now();
        while (std::getline(file, line))
        {
            std::cout << lineNum++ << " ";
            benchMarkRun(line, token, parser, evaluator);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "run time: " << duration.count() << " s" << std::endl
                  << std::endl;

        file.close();
    }

    static void runPrompt()
    {
        int lineNum = 1;
        std::string line;
        std::vector<Token> tokens;
        Parser parser;
        Evaluator evaluator;

        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;

            run(line, tokens, parser, evaluator);
        }
    }
    static void runBenchMark()
    {
        int lineNum = 1;
        std::string line;
        std::vector<Token> tokens;
        Parser parser;
        Evaluator evaluator;

        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;

            auto start = std::chrono::high_resolution_clock::now();
            benchMarkRun(line, tokens, parser, evaluator);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "run time: " << duration.count() << " s" << std::endl;
        }
    }

    static void benchMarkRun(const std::string &source, std::vector<Token> &tokens,
                             Parser &parser, Evaluator &evaluator)
    {
        // auto lx_start = std::chrono::high_resolution_clock::now();
        Lexer lx(source);
        std::vector<Token> new_tokens = lx.scanTokens();
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
        // auto lx_end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = lx_end - lx_start;
        // std::cout << "lx time: " << duration.count() << " s" << std::endl;

        if ((--tokens.end())->type == TokenType::SEMICOLON)
        {
            // auto parse_start = std::chrono::high_resolution_clock::now();
            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();
            // auto parse_end = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> parse_duration = parse_end - parse_start;
            // std::cout << "parse time: " << parse_duration.count() << " s" << std::endl;

            auto program = parser.m_program;

            // auto eval_start = std::chrono::high_resolution_clock::now();
            static Scope global_scp;
            auto evaluated = evaluator.eval(program, global_scp);
            // auto eval_end = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> eval_duration = eval_end - eval_start;
            // std::cout << "evaluate time: " << eval_duration.count() << " s" << std::endl;

            if (evaluated)
                std::cout << evaluated->str() << std::endl;
        }
    }

    static void run(const std::string &source, std::vector<Token> &tokens,
                    Parser &parser, Evaluator &evaluator)
    {
        Lexer lx(source);
        std::vector<Token> new_tokens = lx.scanTokens();
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        for (auto token : tokens)
            std::cout << token.toString();
        std::cout << std::endl;

        if ((--tokens.end())->type == TokenType::SEMICOLON)
        {
            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();

            auto program = parser.m_program;
            auto errors = parser.errors();

            if (!errors.empty())
            {
                for (auto error : errors)
                    std::cout << error << std::endl;
                parser.errors().clear();
            }
            else
            {
                rapidjson::Document root;
                root.SetObject();
                root.AddMember("program", parser.m_program->json(root), root.GetAllocator());
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                root.Accept(writer);
                std::ofstream ofs("ast.json");
                ofs << buffer.GetString();
                ofs.close();

                static Scope global_scp;
                auto evaluated = evaluator.eval(program, global_scp);

                if (evaluated)
                    std::cout << evaluated->str() << std::endl;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cout << "Ewhu Programming Language" << std::endl;
    if (argc > 3)
    {
        std::cerr << "Compile File Usage: Ewhu [script] [output]" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;

        exit(64);
    }
    else if (argc == 3)
    {
        Ewhu::compileFile(argv[1], argv[2]); // 编译传入的脚本文件
    }
    else if (argc == 2)
    {
        if (std::string(argv[1]) == "-b")
        {
            Ewhu::runBenchMark();
        }
        Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    }
    else
    {
        Ewhu::runPrompt(); // 进入交互模式
    }
    return 0;
}
