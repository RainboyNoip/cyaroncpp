#pragma once

#include <type_traits>
#include <fstream>

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

} // namespace cyaron
