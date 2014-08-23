#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>


class MoveMe {
public:
    std::unique_ptr<int, std::function<void (int*)> > m_px;
    MoveMe(int x): m_px(new int{x}, std::default_delete<int>()) {}
    MoveMe(decltype(m_px) px):m_px(std::move(px)){}
    MoveMe(MoveMe &&m):m_px(std::move(m.m_px)) {
        std::cout << "Hello. I have value " << *m_px << " and I have been moved" << std::endl;
    }
};

int main() {
    std::vector<int> ints(10);
    std::iota(std::begin(ints), std::end(ints), 0);
    std::copy(std::begin(ints), std::end(ints), 
              std::ostream_iterator<int>(std::cout, ","));

    std::vector<MoveMe> move_mes;
    move_mes.reserve(10);
    std::for_each(std::begin(ints), std::end(ints),
                   [&](int x) { move_mes.emplace_back(x); });
    move_mes.shrink_to_fit();

    auto backptr = &move_mes.back();
    std::cout << "This is the address of the last element " << backptr << std::endl;
    std::cout << "The last element is " << *backptr->m_px << std::endl;
    
    std::cout << "Constructing one more " << std::endl;
    move_mes.emplace_back(10);

    std::cout << "Original address of the last element: " << backptr << std::endl;
    std::cout << "Current address of the last element: " << &move_mes.back() << std::endl;
    
    std::cout << "Let's try the original: " << *backptr->m_px << std::endl;

    return 0;
}

















