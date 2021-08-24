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
