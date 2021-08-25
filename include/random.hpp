#pragma once

#include <random>

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

    int operator()(){ return dis(engine); }
    //template<typename U> //产生一定范围内的随机数
    long long  operator()(long long l,long long r){ return l == r ? l : dis(engine) % ( r-l+1 ) + l; }

    Random create(int l,int r){ return Random(l,r); } //工厂模式
} RND;

} // namespace cyaron
