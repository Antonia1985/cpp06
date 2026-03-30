#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <ctime>
#include <cstdlib>

int main()
{
    std::srand(std::time(0));
    Base* ptr = generate();
    Base& ref = *ptr;
    identify(ptr);
    identify(ref);
    delete ptr;
    return 0;
}