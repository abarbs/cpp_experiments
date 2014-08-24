#include <iostream>

namespace somenamespace {
class myclass {
    int x;
public:
    myclass(int y):x(y) {}
    /*
      C++11 Standard 7.3.1.2 (3) says:
      
      "If a friend declaration in a nonlocal class first declares a class or function [footnote: this implies that the name of the class or function is unqualified] the friend class or function is a member of the innermost enclosing namespace."

      found at http://stackoverflow.com/a/11509026
    
      Therefore, this operator overload will be found in the namespace "somenamespace"
    */
 
    friend std::ostream& operator<<(std::ostream &os, myclass m) {
        os << m.x;
        return os;
    }
};
}

int main(int argc, char *argv[])
{
    somenamespace::myclass m(5);
    // the custom operator<< will be found by ADL
    std::cout << m << std::endl;
    return 0;
}
