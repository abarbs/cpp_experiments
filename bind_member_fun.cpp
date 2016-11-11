#include <iostream>
#include <functional>
#include <memory>

class Foo {
public:
    Foo(int i): i(i) {}
    int i;

    void print_num(){
        std::cout << this->i << std::endl;
    }
//private:
    Foo(Foo &&f): i(f.i) {}
};

int main()
{
   auto f = std::mem_fn(&Foo::print_num);
   f({3});

   Foo foo{3};
   auto foocopy = std::move(foo);

   std::unique_ptr<int> px(new int(5));
   auto py = std::move(px);

   if (px)
       std::cout << "px is intact with value " << *px << std::endl;
   else if (py)
       std::cout << "py holds the resource with value " << *py << std::endl;

   return 0;
}
