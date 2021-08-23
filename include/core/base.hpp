/*-----------------
* author: Rainboy
* email: rainboylvx@qq.com
* time: 2021年 08月 23日 星期一 22:24:02 CST
*----------------*/

#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>


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
private:
    T value_;
};

#define MAKE__NamedType(__name__,__type__) \
using __name__##Type = NamedType<__type__, struct __##__name__##Tag>;\
static const __name__##Type::argument __name__;


} //namespace cyaron
