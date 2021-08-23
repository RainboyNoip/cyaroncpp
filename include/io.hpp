/* Author:Rainboy */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

#include "traits.hpp"
#include "exec.hpp"



namespace cyaron {
class IO {


public:
    // default ctor
    IO():output_IO{std::cout} ,input_IO{std::cout},
        __input_file_path{""},__output_file_path{""}
    {}
    IO(const std::string & __in,const std::string & __out)
        : __M_output_IO{__out} ,__M_input_IO{__in},
        output_IO{__M_output_IO} ,input_IO{__M_input_IO},
        __input_file_path{__in},__output_file_path{__out}
    {}

    /**
    * 构造函数
    *
    * @param __file_prefix 输出文件的前缀
    *
    * @return 传回
    *
    */
    IO(const std::string & __file_prefix,int id = 1,const std::string& input_suffix= ".in",const std::string& output_suffix = ".out")
        :IO(__file_prefix + "id"+input_suffix,__file_prefix + "id"+output_suffix)
    {}

    template<typename... Args>
    void input_write(Args&&... args){
        __write(input_IO,std::forward<Args>(args)...);
    }

    template<typename... Args>
    void input_writeln(Args&&... args){
        __write(input_IO,std::forward<Args>(args)...);
        input_IO << std::endl;
    }

    template<typename... Args>
    void output_write(Args&&... args){
        __write(output_IO,std::forward<Args>(args)...);
    }

    template<typename... Args>
    void output_writeln(Args&&... args){
        __write(output_IO,std::forward<Args>(args)...);
        input_IO << std::endl;
    }

    void output_gen(const std::string& exe_path ){
        if( __input_file_path.length() == 0){
            throw std::runtime_error("Do not use Class IO default construtor!!");
        }

        __M_input_IO.flush();

        auto __cmd = exe_path + " < " + __input_file_path;
        exec(__cmd.c_str(), __M_output_IO);
    }

    virtual ~IO(){
        close_file();
    }


//private:

    void close_file(){
        if(__input_file_path.length() != 0)
            __M_input_IO.close();
        if(__output_file_path.length() != 0)
            __M_output_IO.close();
    }

    template<typename T>
    constexpr
    void __write_one(std::ostream& __o,T&& __t){
        if constexpr(is_outable<std::remove_cv_t<T>>::value)
            __o << std::forward<T>(__t) << " ";
        else if constexpr ( is_iterable<T>::value )
            for (const auto& e : std::forward<T>(__t)) {
                __write_one(__o,e);
            }
        else
            throw "not out able";
    }

    template<typename... Args>
    void __write(std::ostream & out,Args&&... args){
        //( (out<< args)... );
        ( __write_one(out,std::forward<Args>(args)), ... );
    }

    const std::string __input_file_path;
    const std::string __output_file_path;
    std::ostream& input_IO;
    std::ostream& output_IO;
    std::ofstream __M_input_IO;
    std::ofstream __M_output_IO;

};

} // namespace cyaron