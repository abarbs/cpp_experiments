#include <iostream>
#include <vector>
#include <numeric>

class myclass {
public:
    int x;
    myclass(int x): x(x) {}
};

class some_iterator {
public:
    std::vector<int> x;
    // std::begin found by ADL
    decltype(begin(x)) it;
    decltype(end(x)) ending;

    some_iterator(int y): x(y) {
        std::iota(begin(x), end(x), 0);
        it = begin(x);
        ending = end(x);
    }
    void operator++() { ++it; }
    int& operator*() { return *it; }
    bool operator!=(const some_iterator& other) {
        return it != ending;
    }
    
};

auto begin(myclass m) {
    return some_iterator(m.x);
}

auto end(myclass m) {
    return some_iterator(0);
}

int main(int argc, char *argv[]) {

    myclass m(20);
    for (auto x: m) {
        std::cout << "x is " << x << std::endl;
    }
    
    return 0;
}
