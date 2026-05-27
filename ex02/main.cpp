#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

int main()
{
    std::srand(std::time(0));
    Base* ptr = generate();
    Base& ref = *ptr;
    std::cout << "Class identified by pointer (void identify(Base* p)):" << std::endl;
    identify(ptr);
    std::cout << "Class identified by reference (void identify(Base& p)):" << std::endl;
    identify(ref);
    delete ptr;
    return 0;
}