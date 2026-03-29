#include "Base.hpp"
#include "A.hpp"

Base::Base(){}

Base * generate(void)
{
    Base* a = new A();
    return a;
}

void identify(Base& p)
{
    p.
}