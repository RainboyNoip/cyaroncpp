#include <cyaroncpp/cyaron.hpp>

int main(){
    using namespace cyaron;
    Compare::output(
            stdo="../tests/out1",
            "../tests/out2",
            "../tests/out3",
            "../tests/out4"
            );

    std::cout << "=====IO======" << std::endl;
    IO io("in5","out5");
    for(int i=1;i<=21;++i){
        io.output_writeln(i);
    }
    io.close_file();
    Compare::output(
            stdo = io,
            //stdo = io.get_output_name(),
            "../tests/out1"
            );
    return 0;
}
