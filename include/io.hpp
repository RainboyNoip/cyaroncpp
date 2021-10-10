#pragma once
#ifndef ___CYARON_SINGLE_HPP__
#pragma once
#include "core/base.hpp"
#include "core/traits.hpp"
#include "core/exec.hpp"
#endif


namespace cyaron {
class IO {


public:
    // default ctor
    IO():output_IO{&std::cout} ,input_IO{&std::cout},
        __input_file_path{""},__output_file_path{""}
    {}
    IO(const std::string & __in,const std::string & __out)
        : __M_output_IO{__out} ,__M_input_IO{__in},
        output_IO{&__M_output_IO} ,input_IO{&__M_input_IO},
        __input_file_path{__in},__output_file_path{__out}
    {}

    void operator=(IO&& __io){
        __input_file_path  = std::move(__io.__input_file_path);
        __output_file_path = std::move(__io.__output_file_path);
        //input_IO = std::move(__io.input_IO);
        __M_input_IO = std::move(__io.__M_input_IO);
        __M_output_IO = std::move(__io.__M_output_IO);
        if( __io.input_IO == &__io.__M_input_IO)
            input_IO = &__M_input_IO;
        else
            input_IO = __io.input_IO;

        if( __io.output_IO == &__io.__M_output_IO)
            output_IO = &__M_output_IO;
        else
            output_IO = __io.output_IO;
    }


    /**
    * 构造函数
    *
    * @param __file_prefix 输出文件的前缀
    *
    * @return 传回
    *
    */
    IO(const std::string & __file_prefix,int id = 1,const std::string& input_suffix= ".in",const std::string& output_suffix = ".out")
        :IO(__file_prefix + std::to_string(id)+input_suffix,
                __file_prefix + std::to_string(id)+output_suffix)
    {}

    IO(int id) :IO("data",id)
    {}

    template<typename... Args>
    void input_write(Args&&... args){
        __write(*input_IO,std::forward<Args>(args)...);
    }

    template<typename... Args>
    void input_writeln(Args&&... args){
        __write(*input_IO,std::forward<Args>(args)...);
        (*input_IO) << std::endl;
    }

    template<typename... Args>
    void output_write(Args&&... args){
        __write(*output_IO,std::forward<Args>(args)...);
    }

    template<typename... Args>
    void output_writeln(Args&&... args){
        __write(*output_IO,std::forward<Args>(args)...);
        (*output_IO) << std::endl;
    }

    void output_gen(std::string_view exe_path ){
        if( __input_file_path.length() == 0){
            throw std::runtime_error("Do not use Class IO default construtor!!");
        }

        __M_input_IO.flush();

        auto __cmd = std::string(exe_path) + " < " + __input_file_path;
        exec(__cmd.c_str(), __M_output_IO);
    }

    template<typename T>
    friend IO& operator<<(IO& __io,T&& __t) {
        __io.input_IO << __t;
        return __io;
    }

    //template<typename T>
    //friend IO& operator<<(IO& __io,T&& __t) {
        //__io.output_IO << __t;
        //return __io;
    //}

    std::string get_intput_name() const{
        return __input_file_path;
    }

    std::string get_output_name() const{
        return __output_file_path;
    }

    virtual ~IO(){
        close_file();
    }

    void close_file(){
        if(__input_file_path.length() != 0)
            __M_input_IO.close();
        if(__output_file_path.length() != 0)
            __M_output_IO.close();
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

    std::string __input_file_path;
    std::string __output_file_path;
    std::ostream * input_IO;
    std::ostream * output_IO;
    std::ofstream __M_input_IO;
    std::ofstream __M_output_IO;

};

} // namespace cyaron
