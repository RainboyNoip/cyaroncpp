/* Author:Rainboy */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

namespace cyaron {

class IO {


public:
    IO():output_filename{"out"} ,input_filename{"in"}
    {}
    IO(const std::string & __in,const std::string & __out)
        : output_filename{__out} ,input_filename{__in}
    {}

    void input_write(){
    }

private:

    template<typename... Args>
    void __write(const std::ofstream & out,Args... args){
        ( (out<< args)... );
    }

    std::ofstream input_filename;
    std::ofstream output_filename;

};

} // namespace cyaron
