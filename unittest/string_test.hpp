#include <algorithm>
#include <vector>

#include "random_string.hpp"
#include "tinytest.h"

using namespace cyaron;


void test_random_string(){

    std::string str = "abc";
    auto test = String::random(100,str);
    auto test_random_word = std::all_of(test.begin(), test.end(), [&str](char& a){
            return std::find(str.begin(),str.end(),a) != str.end();
            });
    ASSERT_EQUALS(true, test_random_word);

    //================================
    std::vector<std::string> dict{"lorem", "ipsum", "dolor", "sit", "amet"};
    auto word = String::random(dict);
    auto test_from_dict = std::find(dict.begin(),dict.end(),word) != dict.end();
    ASSERT_EQUALS(true, test_from_dict);

    auto sentence =  String::random_sentence(10,10,sentence_terminators = ".");
    //std::cout << sentence << std::endl;
    ASSERT_EQUALS(true, (bool)std::isupper(sentence[0]));
    ASSERT_EQUALS(true, sentence.back()  == '.');


    //TODO test_ 
}




