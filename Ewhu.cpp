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
private:
public:
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
            std::cerr << "\033[31m" << "Error: Could not open file "
                      << path << "\033[0m" << std::endl;
            exit(1);
        }
        int lineNum = 1;
        while (std::getline(file, line))
        {
            std::cout << std::endl
                      << lineNum++ << " ";
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
            if (!std::getline(std::cin, line)) // if (line == "exit")
            {
                std::cout << "\033[36m" << "( ﾟдﾟ)つBye" << "\033[0m" << std::endl;
                break;
            }
            if (!line.empty())
            {
                run(line, tokens, lexer, parser, evaluator);
            }
        }
    }

    static void runBenchFile(const std::string &path)
    {
        std::string line;
        std::vector<Token> token;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "\033[31m" << "Error: Could not open file "
                      << path << "\033[0m" << std::endl;
            exit(1);
        }
        std::vector<char> buffer(1024 * 1024);
        file.rdbuf()->pubsetbuf(buffer.data(), buffer.size());

        auto start = std::chrono::high_resolution_clock::now();
        while (std::getline(file, line))
        {
            onlyRun(line, token, lexer, parser, evaluator);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "run time: " << duration.count() << "ms" << std::endl;
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
                std::cout << "\033[36m" << "( ﾟдﾟ)つBye" << "\033[0m" << std::endl;
                break;
            }
            benchRun(line, tokens, lexer, parser, evaluator);
        }
    }

    // 不做检查和输出，只运行
    static void onlyRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                        Parser &parser, Evaluator &evaluator)
    {
        lexer.tokens.clear();
        lexer.source = source;
        std::vector<Token> new_tokens = lexer.scanTokens();
        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {
            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();
            auto program = parser.m_program;
            static Scope global_scp;
            auto evaluated = evaluator.eval(program, global_scp);
        }
    }

    // 对每个模块进行测试
    static void benchRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                         Parser &parser, Evaluator &evaluator)
    {

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10000; i++)
        {
            tokens.clear();
            lexer.tokens.clear();
            lexer.source = source;
            std::vector<Token> new_tokens = lexer.scanTokens();
            tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "lexer time: " << duration.count() << "ms" << std::endl;

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 10000; i++)
            {
                parser.new_sentence(tokens.begin(), tokens.end());
                parser.parse_program();
            }
            tokens.clear();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "parser time: " << duration.count() << "ms" << std::endl;

            start = std::chrono::high_resolution_clock::now();
            std::shared_ptr<Object> evaluated;
            for (int i = 0; i < 10000; i++)
            {
                auto program = parser.m_program;
                static Scope global_scp;
                evaluated = evaluator.eval(program, global_scp);
            }
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "evaluate time: " << duration.count() << "ms" << std::endl;
            if (evaluated)
                std::cout << evaluated->str() << std::endl;
        }
    }

    // 正常运行
    static void run(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                    Parser &parser, Evaluator &evaluator)
    {
        lexer.tokens.clear();
        lexer.source = source;
        std::vector<Token> new_tokens = lexer.scanTokens();

        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        // for (auto token : tokens)
        //     std::cout << token.toString();
        // std::cout << lexer.braceStatus << std::endl;

        if ((lexer.braceStatus == 0) &&
            ((--tokens.end())->type == TokenType::SEMICOLON ||
             (--tokens.end())->type == TokenType::RIGHT_BRACE))
        {
            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();

            auto errors = parser.errors();

            if (!errors.empty())
            {
                for (auto error : errors)
                    std::cerr << "\033[31m" << "Error: " << error << "\033[0m" << std::endl;
                parser.m_program->m_statements.clear();
                parser.errors().clear();
            }
            else
            {
                std::cout << "\033[32m" << "Parser: No errorsヾ(✿ﾟ▽ﾟ)ノ" << "\033[0m" << std::endl;
                auto program = parser.m_program;
                jsonOutput(program); // 输出AST
                static Scope global_scp;
                auto evaluated = evaluator.eval(program, global_scp);

                if (evaluated)
                {
                    if (evaluator.is_error(evaluated))
                        std::cerr << "\033[31m" << "Error: " << evaluated->str() << "\033[0m" << std::endl;
                    else
                        std::cout << evaluated->str() << std::endl;
                }
            }
        }
        else
        {
            // std::cerr << "\033[33m" << "Warning: Incomplete statement" << "\033[0m" << std::endl;
        }
    };

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
        std::cout << "\033[32m" << "AST output to ast.jsonヾ(✿ﾟ▽ﾟ)ノ" << "\033[0m" << std::endl;
    }
};
int main(int argc, char *argv[])
{
#ifdef _WIN32
    // 如果是 Windows 平台，设置控制台为 UTF-8 编码
    hl::SetConsoleOutputCP(CP_UTF8);
    hl::SetConsoleCP(CP_UTF8);
#endif
    std::cout << "\033[36m" << "Ewhu Programming Language Ciallo～(∠・ω< )⌒★" << "\033[0m" << std::endl;
    if (argc > 3)
    {
        std::cout << "\033[34m";
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Bench Prompt Usage: Ewhu -b" << std::endl;
        std::cerr << "Bench File Usage: Ewhu -b [script]" << "\033[0m" << std::endl;
        exit(64);
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchFile(argv[2]); // 进入脚本基准测试模式
    }
    else if (argc == 2)
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchPrompt(); // 进入交互基准测试模式
        else
            Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    else
        Ewhu::runPrompt(); // 进入交互模式
    return 0;
}
