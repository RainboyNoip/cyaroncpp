#include <cyaroncpp/cyaron.hpp>

int main(){
    int a = cyaron::RND(1,100);
    int b = cyaron::RND(1,100);
    std::cout << a <<" "<< b << std::endl;
    return 0;
}
