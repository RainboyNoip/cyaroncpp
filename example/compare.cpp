#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "compare.hpp"
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
    Compare::program(
            "1.exe","2.exe","3.exe",
            stdo = "1.out",
            input = "1.in",
            std_program = "hello world"
            );
    //fgogog("123123");
    return 0;
}
