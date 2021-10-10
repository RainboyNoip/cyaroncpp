#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "../include/random.hpp"
#include "../include/compare.hpp"
#endif

int main(){
    using namespace cyaron;
/*
 *    Compare::output(
 *            stdo="../tests/out1",
 *            "../tests/out2",
 *            "../tests/out3",
 *            "../tests/out4"
 *            );
 *
 *    std::cout << "=====IO======" << std::endl;
 *    IO io("in5","out5");
 *    for(int i=1;i<=21;++i){
 *        io.output_writeln(i);
 *    }
 *    io.close_file();
 *    Compare::output(
 *            stdo = io,
 *            //stdo = io.get_output_name(),
 *            "../tests/out1"
 *            );
 */
    std::cout << "============ Compare.program =====" << std::endl;
    Compare::program<true>("std.cpp","std2.cpp",[](cyaron::IO& io){
            int a = RND(1,100);
            int b = RND(1,100);
            //std::cout << a <<" "<< b << std::endl;
            io.input_writeln(a,b);
            },100,"test_1");

    Compare::program<false>("std.cpp","std2.cpp","rnd.cpp",100,"test_2");

    //fgogog("123123");
    return 0;
}
