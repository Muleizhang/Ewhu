#include <iostream>
#include <fstream>
#include <string>
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
        Evaluator evaluator;
        while (std::getline(file, line))
        {
            std::cout << lineNum++ << " ";
            run(line, token, parser, evaluator);
            std::cout << std::endl;
        }

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
            {
                break;
            }
            run(line, tokens, parser, evaluator);
        }
    }

    static void run(const std::string &source, std::vector<Token> &tokens,
                    Parser &parser, Evaluator &evaluator)
    {
        Lexer lx(source);
        std::vector<Token> new_tokens = lx.scanTokens();

        tokens.insert(tokens.end(), new_tokens.begin(), new_tokens.end());

        for (auto token : tokens)
        {
            std::cout << token.toString();
        }

        std::cout << std::endl;

        // 到达分号时，解析语句
        if ((--tokens.end())->type == TokenType::SEMICOLON)
        {
            rapidjson::Document root;
            root.SetObject();
            // std::cout << "analyzing the statement" << std::endl;

            parser.new_sentence(tokens.begin(), tokens.end());
            parser.parse_program();
            tokens.clear();

            auto program = parser.m_program;
            auto errors = parser.errors();
            if (!errors.empty())
            {
                for (auto error : errors)
                {
                    std::cout << error << std::endl;
                }
                parser.errors().clear();
            }
            else
            {
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
                {
                    std::cout << evaluated->str() << std::endl;
                }
            }
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
