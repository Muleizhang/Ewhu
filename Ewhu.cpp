#include <iostream>
#include <fstream>
#include <string>
#include "lexer.cpp"

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
        while (std::getline(file, line))
        {
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

        // 解释 Ewhu 代码的逻辑
        std::cout << "Running: " << source << std::endl;
        Lexer lx(source);
        lx.scan_tokens();
    }
};

int main(int argc, char *argv[])
{

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
