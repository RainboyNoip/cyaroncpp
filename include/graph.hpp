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
MAKE__NamedType_TWO(weight_limit,int)

//TODO std::function<int(int,int)>
//MAKE__NamedType(weight_gen,decltype((int,int)) )


struct __edge { 
    int u,v,w;
    friend std::ostream& operator<<(std::ostream& __out,__edge const & __e){
        __out << __e.u << " " << __e.v << " " << __e.w ;
        return __out;
    }
};

struct graph {

private:

using __LINE__TYPE =  std::vector<__edge> ;

public:
    
    struct Iterator {
        Iterator() = delete;
        explicit Iterator(const unsigned int head_line,const __LINE__TYPE::const_iterator& iter,const graph* father) 
            : head_line{head_line},iter{iter},father{father}
        {}

        void operator++(){
            if( iter == father->edges[father->_M_point_count].end() ) return;
            ++iter;
            if( iter == father->edges[head_line].end()){ //是这一行的结尾
                if( head_line != father->_M_point_count) { // 不是最后一行
                    do {
                        //下一行的开头
                        iter = father->edges[++head_line].begin();
                    }while( 
                            head_line != father->_M_point_count 
                            && iter == father->edges[head_line].end() 
                        );
                }
            }
        }
        void operator++(int){ ++*this; }

        const __LINE__TYPE::value_type& operator*() const{
            return *iter;
        }

        friend bool operator==(const Iterator & __a,const Iterator & __b){
            return !operator==(__a,__b);
        }
        friend bool operator!=(const Iterator & __a,const Iterator & __b){
            return __a.iter != __b.iter;
        }
        private:
            const graph* father;
            unsigned int head_line; //哪个点开头的
            __LINE__TYPE::const_iterator iter;
    };

    graph() = delete;
    graph(unsigned int point_count,bool directed = false)
        : directed{directed} ,_M_point_count{point_count}
    {
        edges.resize(point_count+1);
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

    Iterator begin(){
        for (int i = 1;i<=_M_point_count;++i) {
            auto &v = edges[i];
            if(v.begin() != v.end())
                return Iterator(i,v.begin(),this);
        }
        return end();
    }

    Iterator begin() const{
        for (int i = 1;i<=_M_point_count;++i) {
            auto &v = edges[i];
            if(v.begin() != v.end())
                return Iterator(i,v.begin(),this);
        }
        return end();
    }

    Iterator end(){
        return Iterator(_M_point_count, edges[_M_point_count].end(),this);
    }
    Iterator end() const{
        return Iterator(_M_point_count, edges[_M_point_count].end(),this);
    }

    //添加输出功能
    // TODO 有向图 与 无向图的输出不一样
    // 有向图 所有边都输出
    // 无向图 只输出一次边
    friend std::ostream& operator<<(std::ostream& __out,const graph& __tg) {
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
    unsigned int edges_size() const { return _M_edges_count / ( static_cast<int>(directed) + 1); }
    unsigned int point_count() const{ return _M_point_count; }

    //把回以点u为超点的所有边
    const __LINE__TYPE & edges_by_head(unsigned int u) const{
        return edges[u];
    }
    // static
    template<typename... Args>
    static graph tree(int point_count,Args... args);

    template<typename... Args>
    static graph binary_tree(int point_count,Args... args);
    // static end

private:
    void __add_edge(int u,int v,int w=0){
        ++_M_edges_count;
        edges[u].push_back( {u,v,w} );
    }
    unsigned int _M_point_count;
    unsigned int _M_edges_count{0};
    bool directed = false;
    bool _M_with_weight = true; //输出时带有 weight
    bool _M_with_head   = true; //输出时 先输出 结点数量 与  边数

    std::vector<__LINE__TYPE> edges;
};

template<typename... Args>
graph graph::tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    //std::pair<unsigned int,unsigned int> _M_weight_limit = std::make_pair(3, 8);

    bool _L_directed = __pick_or_default<directedType,Args...>(__args_tuple,false);
    //bool _L_directed = ___pick_or_default<directedType>(__args_tuple,false);
    double _L_chain  = __pick_or_default<chainType>(__args_tuple,0.0);
    double _L_flower = __pick_or_default<flowerType>(__args_tuple,0.0);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(3,8));

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
    
    graph __tg(point_count,_L_directed);

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

} //namespace cyaron 
