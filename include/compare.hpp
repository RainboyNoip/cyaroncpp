#ifndef ___CYARON_SINGLE_HPP__
#include "base.hpp"
#include "io.hpp"
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
    template<typename... T,
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
            std::cout << e << " " << (__ret ? "Right.\n" : "Wrong.\n");
            if( __ret_flag and not __ret) //只有全部的__ret 为true 时，__ret_flag
                __ret_flag = 0;
        }

        return __ret_flag;
    }


    // input
    // stdo
    // std_program
    template<typename... Args>
    static void program(Args&&... args){
        std::vector<std::string> __args{};

        auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);
        std::string _L_stdo        = __pick_or_default<stdoType>(__args_tuple,"out");
        std::string _L_input       = __pick_or_default<inputType>(__args_tuple,"in");
        std::string _L_std_program = __pick_or_default<std_programType>(__args_tuple,"");
        std::cout << _L_stdo << std::endl;
        std::cout << _L_input << std::endl;
        std::cout << _L_std_program << std::endl;

        //进行输出
        if( _L_std_program.length() not_eq 0){
            //TODO check input exists
            // cout some info about this scop code
            if( std::filesystem::path(_L_std_program).is_relative() )
                _L_std_program = std::filesystem::absolute(_L_std_program);
            std::string command = _L_std_program + " < " + _L_input  + " > " + _L_stdo;
            std::cout << "执行命令 : " << command << std::endl;
            exec(command.c_str(),std::cout);
        }

        __pick_string_vec(__args,std::forward<Args>(args)...);

        if( __args.size() == 0){
            throw std::invalid_argument("必须提供用户的程序用于对拍！");
        }

        for (const auto& e : __args) {
            std::string user_exe;
            if( std::filesystem::path(e).is_relative() )
                user_exe = std::filesystem::absolute(e);
            std::string user_out = e +".out";
            std::string command = user_exe + " < " + _L_input  + " > "  + user_out;
            std::cout << "执行命令 : " << command << std::endl;
            exec(command.c_str(),std::cout);
            
            output(stdo = _L_stdo, user_out.c_str());
        }

    }

};

} //namespace cyaron 
