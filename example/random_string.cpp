#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "../include/random.hpp"
#include "../include/random_string.hpp"
#endif

int main(){
    auto t = cyaron::String::random_sentence(10,20,
            cyaron::sentence_terminators   = "[!.]",
            cyaron::first_letter_uppercase = false,
            cyaron::word_separators        = " ",
            cyaron::charset                = "rainboy",
            cyaron::word_length_range      = {2,5}
            );
    std::cout << t << std::endl;

    t = cyaron::String::random_sentence(10,20,
            cyaron::sentence_terminators   = "!",
            cyaron::first_letter_uppercase = true,
            cyaron::word_separators        = " ,-",
            cyaron::charset                = "abcde",
            cyaron::word_length_range      = {3,6}
            );
    std::cout << t << std::endl;
    return 0;
}
