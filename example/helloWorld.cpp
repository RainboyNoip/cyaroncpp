#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "random_string.hpp"
#endif

#include <string>
#include <iostream>

int main(){
    std::vector<std::string> v{"hello world!","hello world."};
    std::cout << 
        cyaron::String::random(v)
        << std::endl;
    return 0;
}
