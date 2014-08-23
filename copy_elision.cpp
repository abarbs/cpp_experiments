#include <string>
#include <iostream>
#include <memory>
#include <map>

class MyClass {
public:
    int myvalue;
    MyClass() : myvalue(0) {}
    MyClass(int v) : myvalue(v) {}
    MyClass(const MyClass& snd) : myvalue(snd.myvalue) {}
    ~MyClass() {
        std::cout << "I'm being destroyed!" << std::endl;
    }
};

MyClass makeMyClass(int v) {
    auto m = MyClass(v);
    std::cout << "Instantiated myclass" << std::endl;
    return m;
}

int main(int argc, char *argv[]) {
    auto m = makeMyClass(10);
    // Copy elision takes place here, so only one MyClass (m) is ever constructed
         
    std::cout << "Done throwing away" << std::endl;
    return 0;
}

















