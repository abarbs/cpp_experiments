#include <iostream>

int& getRef() {
    static int x = 4;
    return x;
}

int main(int argc, char *argv[]) {
    getRef() = 3;
    std::cout << "x is " << getRef() << std::endl;
    return 0;
}
