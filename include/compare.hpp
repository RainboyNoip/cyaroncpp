#ifndef ___CYARON_SINGLE_HPP__
#include "base.hpp"
#include "traits.hpp"
#include "random.hpp"
#endif


namespace cyaron {

const char CR = '\r';
const char LF = '\n';

struct Reader {
    Reader() = delete;
    Reader(const char * file_name)
        :fs(file_name)
    {};
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
    char cur; //当前的char
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

    Reader ouf(std_output_path.c_str());
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

struct Compare {
    template<typename... T,
        std::enable_if_t<
            std::conjunction_v< __has_type<T, const char *,std::string>...  >
            //std::conjunction_v< std::is_same<T, const char *>...  >
            , bool> = true
        >
    static void output(
            stdoType&& __t,
            T... args
            ){

        std::vector<std::string> __args{std::forward<T>(args)...};
        for (const auto& e : __args) {
            std::cout << e << std::endl;
        }
        //for (const auto& e : __outs) {
            //std::cout << e << std::endl;
        //}

        //std::string _L_stdo = __pick_or_default<stdoType>(__args_tuple,"out");
        //__args_tuple

    }

};

} //namespace cyaron 
