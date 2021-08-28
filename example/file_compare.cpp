#include <cyaroncpp/cyaron.hpp>

int main(){
    using namespace cyaron;
    std::ofstream __in;
    __in.open("./in");
    for(int i=1;i<=10;++i){
        __in << i << std::endl;
    }
    __in.close();
    Reader t("./in");
    while ( !t.eof() ) {
        auto line = t.readLine();
        std::cout << line << std::endl;
    }
    return 0;
}
