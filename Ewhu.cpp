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
            run(line, lexer, parser, evaluator);
        }
        file.close();
    }

    static void runPrompt()
    {
        std::string line;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        int lineNum = 1;
        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;
            run(line, lexer, parser, evaluator);
        }
    }

    static void runBenchFile(const std::string &path)
    {
        std::string line;
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
            onlyRun(line, lexer, parser, evaluator);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "run time: " << duration.count() << "ms" << std::endl;
        file.close();
    }

    static void runBenchPrompt()
    {
        std::string line;
        Lexer lexer;
        Parser parser;
        Evaluator evaluator;

        int lineNum = 1;
        while (true)
        {
            std::cout << lineNum++ << " > ";
            if (!std::getline(std::cin, line))
                break;
            benchRun(line, lexer, parser, evaluator);
        }
    }

    // 不做检查和输出，只运行
    static void onlyRun(const std::string &source, Lexer &lexer,
                        Parser &parser, Evaluator &evaluator)
    {
        lexer.tokens.clear();
        lexer.source = source;
        auto new_sentences = lexer.scanTokens();
        for (auto &sentence : new_sentences)
        {
            parser.new_sentence(sentence.begin(), sentence.end());
            parser.parse_program();
            auto program = parser.m_program;
            static Scope global_scp;
            auto evaluated = evaluator.eval(program, global_scp);

            if (!evaluated)
                std::cerr << "Error!" << std::endl;
        }
    }

    // 对每个模块进行测试
    static void benchRun(const std::string &source, Lexer &lexer,
                         Parser &parser, Evaluator &evaluator)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::vector<Token>> new_sentences;
        for (int i = 0; i < 10000; i++)
        {
            lexer.source = source;
            new_sentences = lexer.scanTokens();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "lexer time: " << duration.count() << "ms" << std::endl;

        for (auto &sentence : new_sentences)
        {

            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 10000; i++)
            {
                parser.new_sentence(sentence.begin(), sentence.end());
                parser.parse_program();
            }
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
    static void run(const std::string &source, Lexer &lexer,
                    Parser &parser, Evaluator &evaluator)
    {
        lexer.tokens.clear();
        lexer.source = source;
        auto new_sentences = lexer.scanTokens();
        // for (auto token : tokens)
        //     std::cout << token.toString();
        // std::cout << lexer.braceStatus << std::endl;

        for (auto &sentence : new_sentences)
        {
            parser.new_sentence(sentence.begin(), sentence.end());
            parser.parse_program();
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
        std::cerr << "Run Prompt Usage: Ewhu" << std::endl;
        std::cerr << "Run File Usage: Ewhu [script]" << std::endl;
        std::cerr << "Bench Prompt Usage: Ewhu -b" << std::endl;
        std::cerr << "Bench File Usage: Ewhu -b [script]" << std::endl;
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
