#include <iostream>
using namespace std;
int f(int a)
{
    cout << a << endl;
    return f(a + 1);
}
int main()
{
    f(1);
}