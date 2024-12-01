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

    static void runBenchMark(const std::string &path)
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

    // 不做检查和输出，只运行
    static void onlyRun(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                        Parser &parser, Evaluator &evaluator)
    {
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
            if (!evaluated)
                std::cout << "Error: No output" << std::endl;
        }
    }

    static void run(const std::string &source, std::vector<Token> &tokens, Lexer &lexer,
                    Parser &parser, Evaluator &evaluator)
    {
        lexer.source = source;
        std::vector<Token> new_tokens = lexer.scanTokens();
        lexer.tokens.clear();
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
                    std::cout << error << std::endl;
                parser.errors().clear();
            }
            else
            {
                auto program = parser.m_program;
                jsonOutput(program); // 输出AST
                static Scope global_scp;
                auto evaluated = evaluator.eval(program, global_scp);

                if (evaluated)
                    std::cout << evaluated->str() << std::endl;
            }
        }
        else
        {
            std::cout << "Error: Incomplete statement" << std::endl;
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
        std::cerr << "Bench Mark Usage: Ewhu -b [script]" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;
        exit(64);
    }
    else if (argc == 3)
    {
        if (std::string(argv[1]) == "-b")
            Ewhu::runBenchMark(argv[2]); // 进入基准测试模式
    }
    else if (argc == 2)
        Ewhu::runFile(argv[1]); // 运行传入的脚本文件
    else
        Ewhu::runPrompt(); // 进入交互模式
    return 0;
}
