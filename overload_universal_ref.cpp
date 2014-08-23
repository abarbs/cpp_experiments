#include <iostream>
#include <string>

using namespace std;

class MyClass {
public:
    MyClass() {
        cout << "Regular constructor" << endl;
    }

    MyClass(const MyClass &c) {
        cout << "Copy constructor" << endl;
    }


    MyClass(MyClass&& m) {
        cout << "Move constructor" << endl;
    }
};

// Uncomment this two and leave any of the other three overloads uncommented,
// and the file will fail to compile due to ambiguity in calling
// template <typename T>
// void bad(T t) {
//     cout << "Straight-up T" << endl;
// }

// template <typename T>
// void bad(T& t) {
//     cout << "Ref overload" << endl;
// }

template <typename T>
void bad(const T& t) {
    cout << "Const ref overload" << endl;
}

template <typename T>
void bad(T&& t) {
    cout << "Universal ref overload" << endl;
}

int main()
{
    cout << "Construct m:";
    MyClass m;
    MyClass& lvrm = m;
    const MyClass& lvcrm = m;
    MyClass&& rvrm = move(m);
    const MyClass&& rvrcm = move(m);
    
    cout << endl << "Call bad:" << endl;

    bad(m);
    bad(lvrm);
    bad(lvcrm);
    bad(rvrm);
    bad(rvrcm);

    cout << endl << "Call bad by move:" <<endl;

    bad(std::move(m));
    bad(std::move(lvrm));
    bad(std::move(lvcrm));
    bad(std::move(rvrm));
    bad(std::move(rvrcm));
    
    return 0;
}
