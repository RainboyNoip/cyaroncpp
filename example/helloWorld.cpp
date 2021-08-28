#include <cyaroncpp/cyaron.hpp>
#include <string>
#include <iostream>

int main(){
    std::vector<std::string> v{"hello world!","hello world."};
    std::cout << 
        cyaron::String::random(v)
        << std::endl;
    return 0;
}
