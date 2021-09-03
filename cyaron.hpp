/*-----------------
* author: Rainboy
* email: rainboylvx@qq.com
* time: 2021年 08月 23日 星期一 22:24:02 CST
*----------------*/

#define ___CYARON_SINGLE_HPP__
#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <random>
#include <sstream>
#include <fstream>
#include <string_view>
#include <type_traits>
#include <traits.hpp>


namespace cyaron {
//https://stackoverflow.com/a/41171291
//template <typename T, typename... Us>
//struct __has_type;
template<typename T>
struct __Non_Negative_Integer : std::conjunction<
                                std::is_integral<T>,
                                std::is_unsigned<T>
                                > {};

template <typename T, typename... Us>
struct __has_type : std::disjunction<std::is_same<T, Us>...> {};

const std::string ALPHABET_SMALL = "abcdefghijklmnopqrstuvwxyz";
const std::string ALPHABET_CAPITAL = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string ALPHABET = ALPHABET_SMALL + ALPHABET_CAPITAL;

//[Named Arguments in C++ - Fluent C++](https://www.fluentcpp.com/2018/12/14/named-arguments-cpp/)

template <typename T,typename Parameter>
class NamedType {
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }

    //快速创建一个NamedType类型
    struct argument {
        template<typename UnderlyingType>
        NamedType operator=(UnderlyingType&& value) const {
            return NamedType(std::forward<UnderlyingType>(value));
        }
        argument() = default;
        argument(argument const&) = delete;
        argument(argument&&) = delete;
        argument& operator=(argument const&) = delete;
        argument& operator=(argument&&) = delete;
    };

    //使用std::pair 作为类型
    struct argument_two_args{
        template<typename UnderlyingType>
        NamedType operator=(std::initializer_list<UnderlyingType>&& __list) const {
            if(__list.size() != 2 ){
                throw  std::invalid_argument("给定的参数数量不对,必须有两个参数, 例如: word_length_range = {1,2} \n");
            }
            return NamedType(std::make_pair(
                        *__list.begin(),
                        *(__list.begin()+1)
                        ));
        }

        template<typename UnderlyingType>
        NamedType operator=(std::pair<UnderlyingType,UnderlyingType>&& __mp) const {
            return NamedType(std::forward<std::pair<unsigned int, unsigned int>>(__mp));
        }
    };
private:
    T value_;
};

#define MAKE__NamedType__only(__name__,__type__) \
using __name__##Type = NamedType<__type__, struct __##__name__##Tag>;

#define MAKE__NamedType(__name__,__type__) \
using __name__##Type = NamedType<__type__, struct __##__name__##Tag>;\
static const __name__##Type::argument __name__;

#define MAKE__NamedType_TWO(__name__,__type__) \
using __name__##Type = NamedType<std::pair<__type__,__type__>, struct __##__name__##Tag>;\
static const __name__##Type::argument_two_args __name__;

//得到tuple 里类型的对应类型值
template<typename __getType,typename T>
auto __pick(T & __tuple) -> decltype(std::declval<__getType>().get()){
    return std::get<__getType>(__tuple).get();
}

template <typename T1, typename... T2>
constexpr bool is_tuple_has_type(std::tuple<T2...>) {
  return std::disjunction_v<std::is_same<T1, T2>...>;
}

//得到__NamedType 存储的真正的值的类型
template<typename __getType>
using NamedType_contained_type = std::remove_reference_t<decltype(std::declval<__getType>().get())>;

//进一步封装
// 得到对应类值的值，或，默认值
template<
    typename __getType,
    typename... Args
>
auto __pick_or_default(
        std::tuple<Args...> & __tuple, //参数1
        NamedType_contained_type<__getType>&& default_Ret_Value //参数2 默认值
        ) -> NamedType_contained_type<__getType> //返回引用
{
    using __RetType = NamedType_contained_type<__getType>;
    if constexpr (__has_type<__getType, Args...>::value)
        return std::get<__getType>(__tuple).get();
    else
        return std::forward<__RetType>(default_Ret_Value);
}

//template<
    //typename __getType,
    //typename _Tuple
//>
//auto ___pick_or_default(
        //_Tuple& __tuple, //参数1
        //NamedType_contained_type<__getType>&& default_Ret_Value //参数2 默认值
        //){
    //using __RetType = NamedType_contained_type<__getType>;
    //if constexpr (is_tuple_has_type<__getType>(__tuple))
        //return std::get<__getType>(__tuple).get();
    //else
        //return std::forward<__RetType>(default_Ret_Value);
//}


template<typename T,typename... Args>
void __pick_string_vec(std::vector<std::string>& vs,T&& __t,Args&&... args){
    if constexpr ( std::disjunction_v< __has_type<T, const char *,std::string>,is_const_char_array<T>  >)
        vs.push_back(std::forward<T>(__t));
    if constexpr ( sizeof...(args) == 0)
        return;
    else
        __pick_string_vec(vs,std::forward<Args>(args)...);

}


} //namespace cyaron
#ifndef ___CYARON_SINGLE_HPP__
#pragma once
#include <type_traits>
#include <fstream>
#endif

namespace cyaron {

//copy from [std::void_t - cppreference.com](https://en.cppreference.com/w/cpp/types/void_t)
//
// C++ SFINAE
template <typename T, typename = void>
struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end())>>
    : std::true_type {};

template <typename T, typename = void>
struct is_outable : std::false_type {};

template <typename T>
struct is_outable<T, 
    std::void_t<decltype( std::declval<std::ofstream&>() << std::declval<T>() )>
        > : std::true_type {};


template<size_t N>
constexpr size_t __get_char_array_size(const char (&a)[N]){ return N;}
//SFNIA
template<typename T,typename = void>
struct is_const_char_array : std::false_type {};

template<typename T>
struct is_const_char_array<T,
    std::void_t<decltype( std::declval<std::ofstream&>() << std::declval<T>() )>
        >
: std::true_type {};


} // namespace cyaron
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
#ifndef ___CYARON_SINGLE_HPP__
#pragma once
#include <random>
#endif

namespace cyaron {

struct Random {
    //std::random_device rd;
    std::mt19937 engine{std::random_device ()()};
    std::uniform_int_distribution<long long> dis; // in [0,0x7fffffffffffffff]
    std::uniform_real_distribution<double> unif{0.0,1.0};
    Random(){}
    Random(int l,int r){ dis = std::uniform_int_distribution<long long> (l,r); }
    
    // [0.0, 1.0]
    double random(){
        return unif(engine);
    }

    long long operator()(){ return dis(engine); }
    //template<typename U> //产生一定范围内的随机数
    long long  operator()(long long l,long long r){ return l == r ? l : dis(engine) % ( r-l+1 ) + l; }

    Random create(int l,int r){ return Random(l,r); } //工厂模式
} RND;

} // namespace cyaron
#ifndef ___CYARON_SINGLE_HPP__
#include "base.hpp"
#include "traits.hpp"
#include "random.hpp"
#endif

namespace cyaron {



//using  Sentence_terminators= NamedType<std::string_view, struct __sentence_terminators>;
//static const Sentence_terminators::argument sentence_terminators;
MAKE__NamedType(sentence_terminators,std::string_view)
MAKE__NamedType(word_separators,std::string_view)
MAKE__NamedType(charset,std::string_view)
MAKE__NamedType(first_letter_uppercase,bool)


//word_length_range = {1,2}         or
//word_length_range =  std::make_pair(1, 2);
MAKE__NamedType_TWO(word_length_range, unsigned int)
//using  word_length_range_Type = NamedType<std::pair<unsigned int,unsigned int>, struct __word_length_range>;
//static const struct {
    //word_length_range_Type operator=(std::initializer_list<unsigned int>&& __list) const {
        //if(__list.size() != 2 ){
            //throw  std::invalid_argument("word_length_range 给定的参数数量不对,必须有两个参数, 例如: word_length_range = {1,2} \n");
        //}
        //return word_length_range_Type(std::make_pair(
                    //*__list.begin(),
                    //*(__list.begin()+1)
                    //));
    //}
    //word_length_range_Type operator=(std::pair<unsigned int, unsigned int>&& __mp) const {
        //return word_length_range_Type(std::forward<std::pair<unsigned int, unsigned int>>(__mp));
    //}
//} word_length_range;

struct String {

    static std::string random(int l,int r,std::string_view str = ALPHABET_SMALL ){
        //TODO
        // if str.length() == 0
        // throw std::excep...
        std::string __t = "";
        auto ___size = RND(l,r);
        for(int i=1;i<=___size;++i){
            __t.push_back(str[RND(
                        static_cast<decltype(str.length())>(0)
                        ,str.length()-1)]);
        }
        return __t;
    }

    static std::string random(int n,std::string_view str = ALPHABET_SMALL ){
        return random(n,n,str);
    }

    static std::string random(std::string_view str= ALPHABET_SMALL){
        return random(1,str);
    }
    static std::string random(const std::vector<std::string> & v_str){
        return std::string(v_str[RND(
                    static_cast<decltype(v_str.size())>(0)
                    ,v_str.size()-1)]);
    }

    /**
    * 说明 随机生成一句话
    *
    * 假如
    *  l = 2
    *  r = 10
    *  word_separators=",;"
    *  sentence_terminators=""
    *  first_letter_uppercase=False
    *  word_length_range={2,10}
    *  charset="abcdefg"
    *
    *  生成一个10到20个单词的句子，以逗号或分号随机分割，
    *  第一个单词首字母不大写，结尾没有任何符号，
    *  每个单词2到10字母长，从abcdefg共7个字符中随机选择
    *
    *  @return string
    *
    */
    template<
        typename Integer,
        typename... Args,
        //std::enable_if_t< __Non_Negative_Integer<Integer>::value , bool> = true
        std::enable_if_t< std::is_integral<Integer>::value , bool> = true
        >
    static std::string random_sentence(Integer l,Integer r,Args&&... args){
        if( l < 0 || r < 0 || l > r) {
            throw std::invalid_argument("指定单词数量的参数l,r,必须符合 l>r && l >=0 && r >=0 !");
        }

        std::string_view _M_sentence_terminators      = ".!";
        std::string_view _M_word_separators           = " ";
        std::string_view _M_charset                   = ALPHABET_SMALL;
        bool _M_first_letter_uppercase                = true;
        std::pair<unsigned int,unsigned int> _M_word_length_range = std::make_pair(3, 8);

        auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);
        if constexpr (__has_type<sentence_terminatorsType, std::remove_reference_t<Args>... >::value)
            _M_sentence_terminators = __pick<sentence_terminatorsType>(__args_tuple);

        if constexpr (__has_type<word_separatorsType, std::remove_reference_t<Args>... >::value)
            _M_word_separators = __pick<word_separatorsType>(__args_tuple);

        if constexpr (__has_type<charsetType, std::remove_reference_t<Args>... >::value)
            _M_charset = __pick<charsetType>(__args_tuple);

        if constexpr (__has_type<first_letter_uppercaseType, std::remove_reference_t<Args>... >::value)
            _M_first_letter_uppercase = __pick<first_letter_uppercaseType>(__args_tuple);

        if constexpr (__has_type<word_length_rangeType, std::remove_reference_t<Args>... >::value )
            _M_word_length_range = __pick<word_length_rangeType>(__args_tuple);

        unsigned int __count = RND(l,r);

        std::stringstream ss;
        __count--;
        std::string __tmp = random(_M_word_length_range.first,_M_word_length_range.second,_M_charset);
        if( _M_first_letter_uppercase ) __tmp[0] = std::toupper(__tmp[0]);
        ss << __tmp;
        std::string _real_word_separator = random(1,1,_M_word_separators);

        while ( __count-- ) {
            ss << _real_word_separator;
            ss  << random(_M_word_length_range.first,_M_word_length_range.second,_M_charset);
        }
        if(_M_sentence_terminators.length() != 0)
            ss << random(1, 1, _M_sentence_terminators);

        //std::cout << _M_first_letter_uppercase << std::endl;
        //std::cout << _M_charset << std::endl;

        return ss.str();
    }



    //生成一个n个单词的句子，以空格分割，
    //第一个单词首字母自动大写，结尾有句号或感叹号，每个单词3到8个字母长
    //template<typename Integer,
        //std::enable_if_t<std::is_integral<Integer>::value, bool> = true,
        //std::enable_if_t<std::is_unsigned<Integer>::value, bool> = true
        //>
    static std::string random_sentence(unsigned int n){
        std::stringstream ss;
        const std::string _M_word_separators = " ";
        const std::string _M_sentence_terminators = ( n == 0 ? "" : random(1,"!.") );
        while ( n-- ) {
            ss << random(RND(3,8)) ;
            ss <<  (n == 0 ? "" : _M_word_separators);
        }
        ss << _M_sentence_terminators;
        return ss.str();
    };

    static std::string random_paragraph(){
        return "";
    }

};

//template<>
//std::string String::random_sentence<int>(int n){
    //return "spcial";
//}

} // namespace cyaron

/* Author:Rainboy */
#ifndef ___CYARON_SINGLE_HPP__
#pragma once
#include "base.hpp"
#include "traits.hpp"
#include "exec.hpp"
#endif


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
        :IO(__file_prefix + std::to_string(id)+input_suffix,
                __file_prefix + std::to_string(id)+output_suffix)
    {}

    IO(int id) :IO("data",id)
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
        output_IO << std::endl;
    }

    void output_gen(const std::string& exe_path ){
        if( __input_file_path.length() == 0){
            throw std::runtime_error("Do not use Class IO default construtor!!");
        }

        __M_input_IO.flush();

        auto __cmd = exe_path + " < " + __input_file_path;
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

    const std::string __input_file_path;
    const std::string __output_file_path;
    std::ostream& input_IO;
    std::ostream& output_IO;
    std::ofstream __M_input_IO;
    std::ofstream __M_output_IO;

};

} // namespace cyaron
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
#ifndef ___CYARON_SINGLE_HPP__
#include "base.hpp"
#include "traits.hpp"
#include "random.hpp"
#endif


namespace cyaron {

MAKE__NamedType(chain,double)
MAKE__NamedType(flower,double)
MAKE__NamedType(left,double)
MAKE__NamedType(right,double)
MAKE__NamedType(directed,bool)
MAKE__NamedType(self_loop,bool)
MAKE__NamedType(repeated_edges,bool)
MAKE__NamedType_TWO(weight_limit,int)

//TODO std::function<int(int,int)>
//MAKE__NamedType(weight_gen,decltype((int,int)) )

const unsigned int __End_Sentiel = std::numeric_limits<unsigned int>::max();


struct __edge { 
    int u,v,w;
    decltype(__End_Sentiel) nxt;
    
    friend std::ostream& operator<<(std::ostream& __out,__edge const & __e){
        __out << __e.u << " " << __e.v << " " << __e.w ;
        return __out;
    }

    struct __edge_thin {
        int u,v,w;
        friend std::ostream& operator<<(std::ostream& __out,__edge_thin const & __e){
            __out << __e.u << " " << __e.v << " " << __e.w ;
            return __out;
        }
    };
    static __edge_thin get(__edge const & __e){
        return {__e.u,__e.v,__e.w};
    }

};

struct Graph {

private:

//using __LINE__TYPE =  std::vector<__edge> ;
using __Storage_edge_type = std::list<__edge>;

public:
    
    struct Iterator_for_common_head {
        Iterator_for_common_head() = delete;
        explicit
        Iterator_for_common_head(const unsigned int head_num,Graph const * THIS_pg):
            _M_pg{THIS_pg},_M_edge_idx{THIS_pg->head[head_num]}
        {}
        Iterator_for_common_head& begin() {
            return *this;
        };

        void operator++() {
            _M_edge_idx = _M_pg->__next_edges_idx(_M_edge_idx);
        }

        unsigned int end() const { return __End_Sentiel; };

        bool operator!=(unsigned int __end) const {
            return _M_edge_idx != __end;
            //return __end != __End_Sentiel;
        }
        const __edge& operator*() const{
            return _M_pg->edges[_M_edge_idx];
        }
    private:
        unsigned int _M_edge_idx;
        Graph const  * _M_pg;

    };

    struct Iterator_all {
        using CIter = std::vector<__edge>::const_iterator;
        Iterator_all() = delete;
        Iterator_all(const CIter& iter, Graph const * __graph )
            :iter{iter},_M_pg{__graph}
        {};
        bool operator!=(const CIter& __end) const{ return iter != __end; }
        void operator++(){ ++iter; 
            if( !_M_pg->directed && *this != _M_pg->end())
                ++iter;
        }
        void operator++(int){ ++*this; }

        auto operator*() ->__edge::__edge_thin
        { return  __edge::get(*iter); }
    private:
        CIter iter;
        Graph const  * _M_pg;
    };

    Graph() = delete;
    Graph(unsigned int point_count,bool directed = false)
        : directed{directed} ,_M_point_count{point_count}
    {
        head.resize(point_count+1,__End_Sentiel); //设置大小，所有的元素为-1
    }

    // TODO  转成dot 格式
    std::string to_dot(){
        return "";
    }

    void add_edge(int u,int v,int w = 0){
        __add_edge(u,v,w);
        if( !directed && u != v) // TODO 去除自环
            __add_edge(v,u,w);
    }

    auto begin() const
        -> Iterator_all {
        return Iterator_all(edges.begin(),this);
    }

    //auto begin() {
        //return edges.begin();
    //}

    //auto end(){
        //return  edges.end();
    //}
    auto end() const
        -> Iterator_all::CIter {
        return  edges.end();
    }

    //添加输出功能
    // TODO 有向图 与 无向图的输出不一样
    // 有向图 所有边都输出
    // 无向图 只输出一次边
    friend std::ostream& operator<<(std::ostream& __out,const Graph& __tg) {
        if( __tg._M_with_head)
            __out << __tg.point_count() << ' ' << __tg.edges_size() << '\n';
        for (const auto& e : __tg) {
            auto [u,v,w] = e;
            __out << u << ' ' << v;
            if( __tg._M_with_weight ) __out << ' ' << w;
            __out << '\n';
        }
        return __out;
    }


    //得到边的数量,这个和有向图与无向图有关
    unsigned int edges_size() const { return directed ? _M_edges_count : (_M_edges_count) / 2; }
    unsigned int point_count() const{ return _M_point_count; }

    //TODO 把回以点u为超点的所有边
    auto edges_by_head(unsigned int u) const {
        return Iterator_for_common_head(u,this);
    }
    
    // static
    template<typename... Args>
    static Graph tree(int point_count,Args... args);

    template<typename... Args>
    static Graph binary_tree(int point_count,Args... args);

    template<typename... Args>
    static Graph graph(int point_count,int edge_count,Args... args);

    //保证连通的图,且限定edge_count的范围
    template<typename... Args>
    static Graph graph_connected(int point_count,int edge_count,Args... args);
    // static end
    //
    void set_with_head(bool with = false){ _M_with_head = with; }
    void set_with_weight(bool with = false){_M_with_weight = with;}

private:
    auto __next_edges_idx(unsigned int edges_idx) const
        ->decltype(__End_Sentiel)
    {
        return edges_idx == __End_Sentiel ? __End_Sentiel
                            : edges[edges_idx].nxt;
    }
    void __add_edge(int u,int v,int w=0){
        edges.push_back( {u,v,w,head[u]} );
        head[u] = _M_edges_count++;
    }
    unsigned int _M_point_count;
    unsigned int _M_edges_count{0};
    bool directed = false;
    bool _M_with_weight = true; //输出时带有 weight
    bool _M_with_head   = true; //输出时 先输出 结点数量 与  边数

    //std::vector<__LINE__TYPE> edges;
    std::vector<unsigned int> head;
    std::vector<__edge> edges; //使用vecotor 做为前向星结构
    //std::list<__edge> edges; //使用 链表 做为前向星结构
};

template<typename... Args>
Graph Graph::tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    //std::pair<unsigned int,unsigned int> _M_weight_limit = std::make_pair(3, 8);

    //bool _L_directed = __pick_or_default<directedType,Args...>(__args_tuple,false);
    //bool _L_directed = ___pick_or_default<directedType>(__args_tuple,false);
    double _L_chain  = __pick_or_default<chainType>(__args_tuple,0.0);
    double _L_flower = __pick_or_default<flowerType>(__args_tuple,0.0);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(1,1));

    //debug
    //std::cout << _L_directed << std::endl;
    //std::cout << _L_chain<< std::endl;
    //std::cout << _L_flower << std::endl;
    //std::cout << _L_weight_limit.first << std::endl;
    //std::cout << _L_weight_limit.second<< std::endl;
    if( _L_chain > 1 || _L_chain < 0  ||
            _L_flower > 1 || _L_flower < 0 )
        throw  std::invalid_argument("chain and flower must be between 0 and 1!");
    if( _L_chain + _L_flower > 1)
        throw  std::invalid_argument("chain plus flower must be smaller than 1!");
    
    Graph __tg(point_count,true); //

    int chain_count = static_cast<int>((point_count-1)*_L_chain);
    int flower_count = static_cast<int>((point_count-1)*_L_flower);

    if(chain_count > point_count-1) chain_count = point_count-1;
    if(chain_count + flower_count > point_count - 1)
        flower_count = point_count - 1 - chain_count;
    int random_count = point_count - 1 - chain_count - flower_count;

    for(int i=2;i<=chain_count+1;++i){
        __tg.add_edge(i-1,i,RND(_L_weight_limit.first ,_L_weight_limit.second ));
    }
    for(int i=chain_count+2;i<=chain_count+flower_count+1;++i){
        __tg.add_edge(1,i,RND(_L_weight_limit.first ,_L_weight_limit.second ));
    }
    for(int i = point_count - random_count + 1 ; i<=point_count ; ++i ){
        int u = RND(1,i-1);
        __tg.add_edge(u, i,RND(_L_weight_limit.first ,_L_weight_limit.second ));
    }

    return __tg;
}

template<typename... Args>
Graph Graph::binary_tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    double _L_left  = __pick_or_default<leftType>(__args_tuple,0.0);
    double _L_right = __pick_or_default<rightType>(__args_tuple,0.0);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(1,1));

    if( _L_left < 0 || _L_left > 1 || _L_right < 0 || _L_right > 1){
        throw  std::invalid_argument("left and right must be between 0 and 1");
    }
    if ( _L_left + _L_right > 1)
        throw  std::invalid_argument("left plus right must be smaller than 1");
    
    std::set<unsigned int> can_left{1};
    std::set<unsigned int> can_right{1};

    Graph __tg(point_count,true); //
    for(int i=2;i<=point_count;++i){
        double edge_pos = RND.random();
        //left
        int node = 0;
        if( edge_pos < _L_left || 
            ( _L_left + _L_right < edge_pos  && edge_pos <= (1.0-_L_left - _L_right)/2 ) 
          ){ 
            auto it = can_left.begin();
            std::advance(it,RND(0,can_left.size()-1));
            node = *it;
            can_left.erase(it);
        }
        else { // right
            auto it = can_right.begin();
            std::advance(it,RND(0,can_right.size()-1));
            node = *it;
            can_right.erase(it);
        }
        __tg.add_edge(node, i);
        can_left.insert(i);
        can_right.insert(i);
    }
    return __tg;
}

template<typename... Args>
Graph Graph::graph(int point_count,int edge_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    bool _L_directed = __pick_or_default<directedType>(__args_tuple,false);
    bool _L_self_loop = __pick_or_default<self_loopType>(__args_tuple,true);
    bool _L_repeated_edges = __pick_or_default<repeated_edgesType>(__args_tuple,true);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(3,8));

    Graph __tg(point_count,_L_directed);
    std::map<std::pair<int, int>,bool> used_edges;

    int i = 0;
    while ( i < edge_count ) {
        int u = RND(1,point_count);
        int v = RND(1,point_count);

        if ( ( !_L_self_loop && u == v)  
                || ( !_L_repeated_edges  && used_edges[std::make_pair(u, v)] == 1) )
            continue;

        __tg.add_edge(u, v,RND(_L_weight_limit.first,_L_weight_limit.second));

        if( !_L_repeated_edges){
            used_edges[std::make_pair(u, v)] = 1;
            if( !_L_directed )
                used_edges[std::make_pair(v, u)] = 1;
        }
        ++i;
    }
    return __tg;

}

} //namespace cyaron 
