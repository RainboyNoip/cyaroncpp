#ifndef ___CYARON_SINGLE_HPP__
#pragma once
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#endif

namespace cyaron {

//[C语言中的popen()函数_为幸福写歌的博客-CSDN博客_popen函数](https://blog.csdn.net/yzy1103203312/article/details/78483566)
//[process - How do I execute a command and get the output of the command within C++ using POSIX? - Stack Overflow](https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po)

void exec(const char* cmd,std::ostream& __out) {
    char buffer[128];
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error(std::string("popen() failed!") + __FILE__ + " line: " +  std::to_string(__LINE__).c_str());
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            __out << buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    //return result;
}

} //namespace cyaron
