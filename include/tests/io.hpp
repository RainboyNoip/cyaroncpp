/* Author:Rainboy */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>

#include "traits.hpp"



namespace cyaron {
class IO {


public:
    IO():output_filename{std::cout} ,input_filename{std::cout}
    {}
    IO(const std::string & __in,const std::string & __out)
        : __M_output_filename{__out} ,__M_input_filename{__in},
        output_filename{__M_output_filename} ,input_filename{__M_input_filename}
    {}

    template<typename... Args>
    void input_write(Args&&... args){
        __write(input_filename,std::forward<Args>(args)...);
    }

    template<typename... Args>
    void input_writeln(Args&&... args){
        __write(input_filename,std::forward<Args>(args)...);
        input_filename << std::endl;
    }


private:

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

    std::ostream& input_filename;
    std::ostream& output_filename;
    std::ofstream __M_input_filename;
    std::ofstream __M_output_filename;

};

} // namespace cyaron
