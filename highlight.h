#include <iostream>
#include <string>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
// 自定义 getch 实现
int getch(void)
{
    struct termios oldt, newt;
    int ch;

    // 获取当前终端设置
    tcgetattr(STDIN_FILENO, &oldt);

    // 设置新的终端设置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // 关闭行缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // 读取一个字符
    ch = getchar();

    // 恢复原来的终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
#endif

void moveTo(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);
}

// 高亮输入的字符
void highlightInput(const char c)
{
    printf("%d", c);
    if (std::isdigit(c))
    {
        // light blue
        std::cout << "\033[36m" << c << "\033[0m";
    }
    else if (std::isalpha(c))
    {
        // yellow
        std::cout << "\033[33m" << c << "\033[0m";
    }
    else if (c == '(' || c == ')' || c == '{' || c == '}')
    {
        // green
        std::cout << "\033[32m" << c << "\033[0m";
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
    {
        // red
        std::cout << "\033[31m" << c << "\033[0m";
    }
    else
    {
        std::cout << "\033[90m" << c << "\033[0m";
    }
}

// 自定义输入函数
std::string customInput(int lineNum)
{
    std::string input;
    char c;
    std::cout << lineNum << " > ";

    while (true)
    {
        c = getch(); // 非阻塞读取单个字符

        if (c == '\r' || c == '\n') // Enter 键处理
        {
            std::cout << "\n";
            return input;
        }
        else if (c == 8 || c == 127) // 退格键处理（Windows: 8, Linux: 127）
        {
            if (!input.empty())
            {
                input.pop_back();
                std::cout << "\b \b"; // 删除一个字符
            }
        }
        else if (c == -32)
        {
            c = getch();
            printf("%d", c);
            if (c == 75)
            {
                moveTo(10, 5);
            }
        }
        else
        {
            input += c; // 添加到输入缓冲
            highlightInput(c);
        }
    }
}
