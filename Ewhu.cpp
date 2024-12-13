#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "evaluator/evaluator.h"

#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

#ifdef _WIN32
namespace hl
{
#include <windows.h>
#include <conio.h>
}
#endif

class Ewhu
{
public:
    inline static void printUsage()
    {
        std::cerr << "\033[34m";
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Bench Prompt Usage: Ewhu -b" << std::endl;
        std::cerr << "Bench File Usage: Ewhu -b [script]" << "\033[0m" << std::endl;
    }
    template <typename... Msgs>
    inline static void printError(const Msgs &...msgs)
    {
        (std::cerr << "\033[31m" << ... << msgs) << "\033[0m" << std::endl;
    }

    template <typename... Msgs>
    inline static void printGreen(const Msgs &...msgs)
    {
        (std::cout << "\033[32m" << ... << msgs) << "\033[0m" << std::endl;
    }

    template <typename... Msgs>
    inline static void printBlue(const Msgs &...msgs)
    {
        (std::cout << "\033[36m" << ... << msgs) << "\033[0m" << std::endl;
    }

    template <typename T>
    inline static void bench(T Function, int times = 10000)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < times; i++)
        {
            Function();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "run time: " << duration.count() << "ms" << std::endl;
    }

public:
    static void runFile(const std::string &path)
    {
        std::string line;
        std::vector<Token> tokens;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        std::ifstream file(path);
        if (!file.is_open())
        {
            printError("Error: Could not open file " + path);
            exit(1);
        }
        int lineNum = 1;
        while (std::getline(file, line))
        {
            lineNum++;
            if (!line.empty())
            {
                try
                {
                    run(line, tokens, lexer, parser, evaluator);
                }
                catch (const std::exception &e)
                {
                    printError(lineNum, ": ", line);
                    printError(e.what());
                }
            }
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
            std::cout << ++lineNum << " > ";
            if (!std::getline(std::cin, line))
            {
                printBlue("( ﾟдﾟ)つBye");
                exit(-1);
            }
            if (!line.empty())
            {
                try
                {
                    run(line, tokens, lexer, parser, evaluator);
                }
                catch (const std::exception &e)
                {
                    printError(lineNum, ": ", line);
                    printError(e.what());
                }
            }
        }
    }

    static void runBenchFile(const std::string &path)
    {
        std::string line;
        std::vector<Token> tokens;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        std::ifstream file(path);
        if (!file.is_open())
        {
            printError("Error: Could not open file " + path);
            exit(1);
        }
        std::vector<char> buffer(1024 * 1024);
        file.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

        bench([&]()
              {while (std::getline(file, line)){onlyRun(line, tokens, lexer, parser, evaluator);} });

        file.close();
    }

    static void runBenchPrompt()
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
            {
                printBlue("( ﾟдﾟ)つBye");
                break;
            }
            benchRun(line, tokens, lexer, parser, evaluator);
        }
    }

    // 不做检查和输出，只运行
    static void onlyRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                        Parser &parser, Evaluator &evaluator)
    {
        std::vector<Token> new_tokens = lexer.scanTokens(source);
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {
            parser.parse_program(tokens);
            tokens.clear();
            auto program = parser.m_program;
            static Scope global_scp;
            auto evaluated = evaluator.eval_program(program, global_scp);
        }
    }

    // 对每个模块进行测试
    static void benchRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                         Parser &parser, Evaluator &evaluator)
    {
        bench(
            [&]()
            {
                tokens.clear();
                std::vector<Token> new_tokens = lexer.scanTokens(source);
                tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
            });

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {
            bench(
                [&]()
                {
                    parser.parse_program(tokens);
                });
            tokens.clear();
            bench(
                [&]()
                {
                    static Scope global_scp;
                    auto evaluated = evaluator.eval_program(parser.m_program, global_scp);
                    if (evaluated)
                        std::cout << evaluated->str() << std::endl;
                });
        }
    }

    // 正常运行
    static void run(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                    Parser &parser, Evaluator &evaluator)
    {
        lexer.scanTokens(source);
        std::vector<Token> new_tokens = lexer.scanTokens(source);
        for (auto &token : new_tokens)
        {
            std::cout << token.toString() << " ";
        }
        std::cout << std::endl;
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {
            parser.parse_program(tokens);
            tokens.clear();

            auto &program = parser.m_program;
            jsonOutput(program);

            printGreen("evaluatingヾ(✿ﾟ▽ﾟ)ノ");
            static Scope global_scp;
            auto evaluated = evaluator.eval_program(program, global_scp);
            if (evaluated)
                std::cout << evaluated->str() << std::endl;
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
        printGreen("AST output to ast.json");
    }
};

int main(int argc, char *argv[])
{
#ifdef _WIN32
    // 如果是 Windows 平台，设置控制台为 UTF-8 编码
    hl::SetConsoleOutputCP(CP_UTF8);
    hl::SetConsoleCP(CP_UTF8);
#endif

    Ewhu::printBlue("Ewhu Programming Language Ciallo～(∠・ω< )⌒★");
    if (argc > 3)
    {
        Ewhu::printError("Error: Too many arguments");
        Ewhu::printUsage();
        exit(64);
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchFile(argv[2]); // 进入脚本测试模式
    }
    else if (argc == 2)
    {
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchPrompt(); // 进入交互测试模式
        else
            Ewhu::runFile(argv[1]); // 进入脚本模式
    }
    else if (argc == 1)
        Ewhu::runPrompt(); // 进入交互模式
    return 0;
}
