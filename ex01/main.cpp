#include "Serializer.hpp"
#include <iostream>

int main(int ac, char** agv)
{
    if (ac != 2)
    {
        std::cout << "Usage: ./serialize <your_string>" << std::endl;
        return 1;
    }

    Data d;
    d.str = agv[1];
    Data* ptr = &d;
    
    uintptr_t i = Serializer::serialize(ptr);
    Data * ptr2 = Serializer::deserialize(i);

    std::cout << "string value: " << d.str << std::endl;
    std::cout << "original ptr: " << ptr << std::endl;
    std::cout << "uintptr_t: " << i << std::endl;
    std::cout << "copy ptr2: " << ptr2 << std::endl;
    std::cout << "ptr2->str: " << ptr2->str << std::endl;
    std::cout << "ptr == ptr2: " << (ptr == ptr2 ? "true" : "false") << std::endl;
    return 0;
}