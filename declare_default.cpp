#include <iostream>

int f(int x=3);
int f(int x) {
    return x + 1;
}

int main()
{
    std::cout << f() << std::endl;
    return 0;
}
