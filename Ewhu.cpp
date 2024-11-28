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
        std::string line;
        std::vector<Token> token;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << path << std::endl;
            exit(-1);
        }
        int lineNum = 1;
        while (std::getline(file, line))
        {
            std::cout << lineNum++ << " ";
            run(line, token, lexer, parser, evaluator);
        }
        file.close();
    }

    static void runPrompt()
    {
        std::string line;
        std::vector<Token> tokens;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        int lineNum = 1;
        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;

            run(line, tokens, lexer, parser, evaluator);
        }
    }
    static void runBenchMark(int times = 1)
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

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < times; i++)
            {
                onlyRun(line, tokens, lexer, parser, evaluator);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            std::cout << "run time: " << duration.count() << " s" << std::endl;
        }
    }

    // 完全不做检查和输出，只运行
    static void onlyRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                        Parser &parser, Evaluator &evaluator)
    {
        // std::cout << "source: " << source << std::endl;
        lexer.source = source;
        std::vector<Token> new_tokens = lexer.scanTokens();
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
        if ((--tokens.end())->type == TokenType::SEMICOLON)
        {
            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();
            auto program = parser.m_program;
            static Scope global_scp;
            auto evaluated = evaluator.eval(program, global_scp);
        }
    }

    static void run(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                    Parser &parser, Evaluator &evaluator)
    {
        lexer.source = source;
        std::vector<Token> new_tokens = lexer.scanTokens();
        lexer.tokens.clear();
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
                jsonOutput(program);
                static Scope global_scp;
                auto evaluated = evaluator.eval(program, global_scp);

                if (evaluated)
                    std::cout << evaluated->str() << std::endl;
            }
        }
    }

    static void jsonOutput(std::shared_ptr<Program> program)
    {
        rapidjson::Document root;
        root.SetObject();
        root.AddMember("program", program->json(root), root.GetAllocator());
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        root.Accept(writer);
        std::ofstream ofs("ast.json");
        ofs << buffer.GetString();
        ofs.close();
    }
};

int main(int argc, char *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cout << "Ewhu Programming Language" << std::endl;
    if (argc > 3)
    {
        std::cerr << "Bench Mark Usage: Ewhu -b [times]" << std::endl;
        std::cerr << "Compile File Usage: Ewhu [script] [output]" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;

        exit(64);
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchMark(std::stoi(argv[2])); // 进入基准测试模式
        else
            Ewhu::compileFile(argv[1], argv[2]); // 编译传入的脚本文件
    }
    else if (argc == 2)
        Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    else
        Ewhu::runPrompt(); // 进入交互模式
    return 0;
}
