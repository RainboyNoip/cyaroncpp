#pragma once
#include <filesystem>
#include <thread>
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
#include <chrono>
#include "traits.hpp"


namespace cyaron { //utils

template <typename I> 
std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

std::string simple_uuid(){
    return n2hexstr( std::chrono::system_clock::now().time_since_epoch().count() );
}

} //namespace cyaron utils


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
