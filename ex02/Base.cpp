#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <iostream>
#include <cstdlib>

Base::~Base(){}

Base * generate(void)
{
    Base* ret;
    int num = std::rand()%3;
    if (num == 0)
    {
        ret = new A();
    }
    else if (num == 1)
    {
        ret = new B();
    }
    else
    {
        ret = new C();
    }
    return ret;
}

void identify(Base* p)
{
    if(dynamic_cast<A*>(p) == NULL)
    {
        if(dynamic_cast<B*>(p) == NULL)
        {
            if(dynamic_cast<C*>(p) == NULL)
            {
                std::cout << "Casting Failed" << std::endl;
            }
            else
            {
                std::cout << "Type is class C" << std::endl;
            }
        }
        else 
        {
            std::cout << "Type is class B" << std::endl;
        }
    }
    else 
    {
        std::cout << "Type is class A" << std::endl;
    }    
}

void identify(Base& p)
{
    try
    {
        (void)dynamic_cast<A&>(p);
        std::cout << "Type is class A" << std::endl;
    }
    catch(const std::exception& e)
    {
        try
        {
            (void)dynamic_cast<B&>(p);
            std::cout << "Type is class B" << std::endl;
        }
        catch(const std::exception& e)
        {
            try
            {
                (void)dynamic_cast<C&>(p);
                std::cout << "Type is class C" << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << "Casting Failed" << std::endl;                
            }
        }
    }
}