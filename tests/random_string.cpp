#include "random_string.hpp"

int main(){

    //{
        //using namespace cyaron;
        //auto t = ( word_length_range = {100,123} );
        //std::cout << t.get().first << std::endl;
        //std::cout << t.get().second << std::endl;
    //}
    //cyaron::String::random_sentence(cyaron::sentence_terminators = "hello");
    //std::cout <<cyaron::String::random_sentence(int(5)) ;
    //std::cout << "=====" << std::endl;
    auto t = 
    cyaron::String::random_sentence(10,20,
            cyaron::sentence_terminators   = "[!.]",
            cyaron::first_letter_uppercase = false,
            cyaron::word_separators        = " ",
            cyaron::charset                = "rainboy",
            cyaron::word_length_range      = {2,5}
            );
    std::cout << t << std::endl;

    t = 
    cyaron::String::random_sentence(10,20,
            cyaron::sentence_terminators   = "!",
            cyaron::first_letter_uppercase = true,
            cyaron::word_separators        = " ,-",
            cyaron::charset                = "abcde",
            cyaron::word_length_range      = {3,6}
            );
    std::cout << t << std::endl;
    return 0;
}
