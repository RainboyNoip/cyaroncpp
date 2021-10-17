#pragma once
#ifndef ___CYARON_SINGLE_HPP__
#include "core/base.hpp"
#include "io.hpp"
#include "utils.hpp"
#include <functional>
#endif



namespace cyaron {

const char CR = '\r';
const char LF = '\n';

struct Reader {
    Reader() = delete;
    Reader(const char * file_name)
    { fs.open(file_name);}
    virtual ~Reader(){ fs.close(); }
        //当前是否到了eof
    bool eof(){ return cur == EOF; }
    bool isEofn(char c) { return c == CR || c == LF ;}
    void readEoln(){
        for(;;)
            if( isEofn(peekNextChar()) )
                getNextChar();
            else break;
    }

    std::string readLine(){
        std::string __ret;
        for(;;){
            char c = getNextChar();
            if( isEofn(c)) break;
            if( c == EOF ) break;
            __ret += c;
        }
        readEoln();
        return __ret;
    }
private:
    char cur{-2}; //当前的char
    std::ifstream fs;

    //读取下一个字符
    inline char getNextChar(){
        cur = static_cast<char>(fs.get());
        return cur;
    }

    //读取下一个字符
    inline char peekNextChar(){
        return static_cast<char>(fs.peek());
    }
};


bool noipstyle_check(
        const std::string & user_output_path,
        const std::string & std_output_path )
{
    auto del_line_back_space = [](std::string &str){
        while(!str.empty()){
            auto ec = str.back();
            if( ec == '\n' || ec == ' ' || ec == '\r') str.pop_back();
            else break;
        }
    };

    Reader ouf(user_output_path.c_str());
    Reader ans(std_output_path.c_str());
    int n=0;
    while ( !ans.eof() ) {
        std::string j = ans.readLine();
        del_line_back_space(j);            //去除末尾空白
        if (j.empty() && ans.eof()) break; // ans 读到最后一个空白行
        std::string p = ouf.readLine();
        del_line_back_space(p);
        n++;
        if (j != p) return 0;
    }
    return 1;
}

MAKE__NamedType(stdo, std::string)

//可以使用 stdo = io 
template<>
template<>
stdoType stdoType::argument::operator=<IO&>(IO& io) const {
    return stdoType(io.get_output_name());
}

MAKE__NamedType(input, std::string)
template<>
template<>
inputType inputType::argument::operator=<IO&>(IO& io) const {
    return inputType(io.get_intput_name());
}

MAKE__NamedType(std_program, std::string)


// stdoType = NamedType<.....>
//struct newTTT :public stdoType::argument {
    //template<>
    //NamedType<std::string,struct __stdoTag> operator=<IO>(IO const & io) const {
            //return NamedType<std::string,struct __stdoTag>(io.get_output_name());
    //}
//};

struct Compare {
    template<
        bool info = true,
        typename... T,
        //限制T的类型是都是字符串
        std::enable_if_t<
            std::conjunction_v< __has_type<T, const char *,std::string>...  >
            //std::conjunction_v< std::is_same<T, const char *>...  >
            , bool> = true
        >
    static bool output(
            stdoType&& __t,
            T... args
            ){

        std::vector<std::string> __args{std::forward<T>(args)...};
        bool __ret_flag = 1;
        for (const auto& e : __args) {
            //std::cout << e << std::endl;
            //TODO check file exists
            auto __ret = noipstyle_check(
                    e.c_str(),
                    __t.get().c_str()
                    );
            if constexpr ( info )
                std::cout << e << " " << (__ret ? "Right.\n" : "Wrong.\n");
            if( __ret_flag and not __ret) //只有全部的__ret 为true 时，__ret_flag
                __ret_flag = 0;
        }

        return __ret_flag;
    }

    // TODO 给多线程Compare::program 使用
    struct for_compare_program_multiThread {
        //a _exit_ = 0
    };

    using rnd_func_type = std::function<void(IO&)>;
    //单独文件：用户/标准的输出的文件都在一个文件里
    template<bool singleFile = true>
    static void program(std::string_view usr_exe,
            std::string_view std_exe,
            std::variant< rnd_func_type, std::string_view > rnd,
            std::size_t cnt=1000,       //比较次数
            std::filesystem::path compare_path = "compare" //比较的目录
            ){
        std::string USR_EXE ( std::filesystem::absolute( std::filesystem::path(usr_exe).stem() ) );
        std::string STD_EXE ( std::filesystem::absolute( std::filesystem::path(std_exe).stem() ) );
        if( std::filesystem::path(usr_exe).extension() == ".cpp"){
            //compile
            if( not compile(usr_exe) ){
                std::cerr << "编译"<< usr_exe <<" 失败！" << std::endl;
                return;
            }
        }
        if( std::filesystem::path(std_exe).extension() == ".cpp"){
            if( not compile(std_exe) ){
                std::cerr << "编译"<< std_exe <<" 失败！" << std::endl;
                return; }
        }
        //判断是否是rnd_func
        rnd_func_type rnd_func;
        if (std::holds_alternative<rnd_func_type>(rnd)){
            rnd_func = [&rnd](IO& rndio){
                std::get<rnd_func_type>(rnd)(rndio);    //如果是一个可执行的对象
                rndio.close_file();
            };
        }
        else {  //是一cpp文件名，或可执行文件
            auto rnd_path = std::filesystem::path(std::get<std::string_view>(rnd));
            std::string RND_EXE ( std::filesystem::absolute( rnd_path.stem() ) );
            if( rnd_path.extension() == ".cpp" && not compile(rnd_path.c_str()) ){
                std::cerr << "编译"<< rnd_path <<" 失败！" << std::endl;
                return;
            }
            rnd_func = [RND_EXE](IO& rndio){
                rndio.close_file();
                std::string rnd_cmd = RND_EXE + " > " + rndio.get_intput_name();
                std::stringstream sout;
                exec(rnd_cmd.c_str() , sout);
            };
        }


        //创建一个compare文件夹用于对拍
        std::filesystem::create_directories(compare_path);
        for(int i=1;i<=cnt;++i){
            progress_bar(i,cnt);
            IO rndio;
            if constexpr( singleFile )
                rndio = IO(compare_path / "in","/dev/null");
            else
                rndio = IO(compare_path / ("in"+std::to_string(i)),"/dev/null");

            rnd_func(rndio);

            std::stringstream ss,sout;
            std::string __usr_out,__std_out;
            if constexpr(singleFile ){
                __usr_out = compare_path/"usr_out";
                __std_out = compare_path/"std_out";
            }
            else {
                __usr_out = compare_path /("usr_out_" + std::to_string(i) );
                __std_out = compare_path /("std_out_" + std::to_string(i) );
            }

            ss << USR_EXE << " < ";
            ss << rndio.get_intput_name() << " > ";
            ss << __usr_out;
            try {
                //dbg_one(ss.str());
                exec(ss.str().c_str(),sout);
            }
            catch(...) { std::cout << "运行 "<< USR_EXE << " 出错.";return;}

            ss.str(""); //empty it
            ss << STD_EXE<< " < ";
            ss << rndio.get_intput_name() << " > ";
            ss << __std_out;
            try {
                //dbg_one(ss.str());
                exec(ss.str().c_str(),sout);
            }
            catch(...) { std::cout << "运行 "<< STD_EXE << " 出错.";return;}

            //比较
            if( not output<false>(stdo = __std_out,__usr_out) ){
                std::cout << "出现错误" ;
                break;
            }
        }
        std::cout << std::endl;
    }

    // input 输入数据
    // stdo
    // std_program
    // TODO 对线程对拍
    //static void program(std::string_view usr_exe,
            //std::string_view std_exe,
            //std::string_view rnd_exe,
            //std::size_t cnt=1000
            //){
        //std::vector<std::string> __args{};

        //auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);
        //std::string _L_stdo        = __pick_or_default<stdoType>(__args_tuple,"out");
        //std::string _L_input       = __pick_or_default<inputType>(__args_tuple,"in");
        //std::string _L_std_program = __pick_or_default<std_programType>(__args_tuple,"");
        //std::cout << _L_stdo << std::endl;
        //std::cout << _L_input << std::endl;
        //std::cout << _L_std_program << std::endl;

        ////进行输出
        //if( _L_std_program.length() not_eq 0){
            ////TODO check input exists
            //// cout some info about this scop code
            //if( std::filesystem::path(_L_std_program).is_relative() )
                //_L_std_program = std::filesystem::absolute(_L_std_program);
            //std::string command = _L_std_program + " < " + _L_input  + " > " + _L_stdo;
            //std::cout << "执行命令 : " << command << std::endl;
            //exec(command.c_str(),std::cout);
        //}

        //__pick_string_vec(__args,std::forward<Args>(args)...);

        //if( __args.size() == 0){
            //throw std::invalid_argument("必须提供用户的程序用于对拍！");
        //}

        //for (const auto& e : __args) {
            //std::string user_exe;
            //if( std::filesystem::path(e).is_relative() )
                //user_exe = std::filesystem::absolute(e);
            //std::string user_out = e +".out";
            //std::string command = user_exe + " < " + _L_input  + " > "  + user_out;
            //std::cout << "执行命令 : " << command << std::endl;
            //exec(command.c_str(),std::cout);

            //output(stdo = _L_stdo, user_out.c_str());
        //}

    //}

};

} //namespace cyaron 
