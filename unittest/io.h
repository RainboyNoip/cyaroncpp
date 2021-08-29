#include <algorithm>
#include <vector>
#include <filesystem>

#include "io.hpp"
#include "tinytest.h"


void test_io(){
    using namespace cyaron;
    IO io1;
    std::vector<std::string> files;
    for(int i=1;i<=10;++i){
        IO io(i);
        io.input_writeln(i);
        io << i << '\n';
        //io >> i >> '\n';
        //io.output_writeln(i);
        io.output_gen("./std");
        files.push_back(io.get_intput_name());
        files.push_back(io.get_output_name());
    }
    auto __files_exists = std::all_of(files.begin(), files.end(), 
                [](std::string &name){ 
                    auto exists = std::filesystem::exists(name); 
                    std::cout << name << "\texists:"<<"\t"<< exists << std::endl;
                    return exists;
                }
            );


    ASSERT_EQUALS(true,__files_exists);
}

