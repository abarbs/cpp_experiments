#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

class MyClass
{
public:
    int myval;

    MyClass(int x):myval(x) {
        cout << "constructor called" << endl;
    }
    MyClass(const MyClass &m) = delete;
    MyClass(MyClass &m) = default;

    MyClass(MyClass &&m)  {
        cout << "Rval constructor" << endl;
        this->myval = m.myval;
    };
    
    MyClass(const MyClass &&m) = delete;

};


template<typename T>
void f(T&& t) {
    cout << "Cool! " << endl;
}


int main()
{

    // Different behavior depending on whether "elision" is turned on:
    // If it is (which is the default), the next line is equivalent to
    // instantiating s directly with parameter 3 (temporary never exists),
    // if you compile with -fno-elide-constructors, you'll see that the temporary
    // is constructed, then s is constructed via Rvalue-reference constructor
    MyClass s = MyClass(3);
    
    auto&& rs = s;

    // Reference collapsing: 
    //try ` clang++ -O0 -g -std=c++1y rval_ref.cpp && nm -C a.out | grep "void f"`
    // You'll see that f is instantiated with T = "int&", t has type "int&"
    int x =5 ;
    f(x);

    return 0;
}
