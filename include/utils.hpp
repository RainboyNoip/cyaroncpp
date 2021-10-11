#pragma once

#include <sstream>
#include <filesystem>
#include "core/exec.hpp"

template<char Delimiter = ' ',typename... Args>
void debug_out(std::ostream &os, Args&&... args){
    ( (os << args << Delimiter),... ) <<std::endl;
}
#ifdef LOCALTEST
#define dbg(...) debug_out(std::cout,"FILE:",__FILE__,"[LINE:]",__LINE__,##__VA_ARGS__)
#else
#define dbg(...)
#endif
#define dbg_one(...) dbg(#__VA_ARGS__,"=",__VA_ARGS__)
//8========================== debug end ==========================


enum class COMPILE_TYPE {
    cpp,
    python
};

inline constexpr const auto cpp = COMPILE_TYPE::cpp;
inline constexpr const auto python = COMPILE_TYPE::python;
//编译代码
template<COMPILE_TYPE type = cpp>
bool compile(std::string_view code_path){
    dbg_one(code_path);
    if constexpr (type == cpp){
        std::stringstream ss;
        ss << "g++ -std=c++17 -g -o ";
        ss << std::filesystem::path(code_path).stem() << " ";
        ss << code_path;
        std::stringstream sout;
        dbg_one(ss.str());
        try {
            cyaron::exec(ss.str().c_str(),sout);
        }
        catch(...){
            std::cout << "编译失败！" << std::endl;
            return false;
        }
    }
    else {
        std::cout << "不支持的语言" << std::endl;
        return false;
    }

    return true;
}

//from here https://ideone.com/Yg8NKj
template<int barWidth = 70>
void progress_bar(int now,int total){
    double progress = now*1.0 / total;
    if( progress > 1.0 ) progress = 1.0;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    //std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout << "] " << std::setw(5) << now << '/' << total  << " \r";
    std::cout.flush();
    //std::cout << std::endl; //用来最后的换行
}
