#include "io.hpp"

int main(){
    cyaron::IO io;
    io.input_write(1,2,3,4);
    //std::cout << "helloworld" << std::endl;
    io.input_writeln(1,2,3,4);
    return 0;
}
