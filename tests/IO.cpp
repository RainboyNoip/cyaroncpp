#include "io.hpp"
#include <array>


int main(){
    cyaron::IO io;
    io.input_write(1,2,3,4);
    //std::cout << "helloworld" << std::endl;
    io.input_writeln(1,2,3,4);

    std::array<int, 10> a{1,2,3,4};
    io.input_writeln(a);

    std::cout << "== 开始测试文件输出=== " << std::endl;
    cyaron::IO io2("1.in","1.out");
    io2.input_writeln(a);
    io2.output_gen("/usr/bin/cat");
    return 0;
}
