#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "../include/random.hpp"
#include "../include/compare.hpp"
#endif

int main(){
    int a = cyaron::RND(1,100);
    int b = cyaron::RND(1,100);
    std::cout << a <<" "<< b << std::endl;
    return 0;
}
