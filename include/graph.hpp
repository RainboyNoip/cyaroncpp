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

const unsigned int __End_Sentiel = std::numeric_limits<unsigned int>::max();


struct __edge { 
    int u,v,w;
    decltype(__End_Sentiel) nxt;
    
    friend std::ostream& operator<<(std::ostream& __out,__edge const & __e){
        __out << __e.u << " " << __e.v << " " << __e.w ;
        return __out;
    }

    struct __edge_thin {
        int u,v,w;
        friend std::ostream& operator<<(std::ostream& __out,__edge_thin const & __e){
            __out << __e.u << " " << __e.v << " " << __e.w ;
            return __out;
        }
    };
    static __edge_thin get(__edge const & __e){
        return {__e.u,__e.v,__e.w};
    }

};

struct graph {

private:

//using __LINE__TYPE =  std::vector<__edge> ;
using __Storage_edge_type = std::list<__edge>;

public:
    
    struct Iterator_for_common_head {
        Iterator_for_common_head() = delete;
        explicit
        Iterator_for_common_head(const unsigned int head_num,graph const * THIS_pg):
            _M_pg{THIS_pg},_M_edge_idx{THIS_pg->head[head_num]}
        {}
        Iterator_for_common_head& begin() {
            return *this;
        };

        void operator++() {
            _M_edge_idx = _M_pg->__next_edges_idx(_M_edge_idx);
        }

        unsigned int end() const { return __End_Sentiel; };

        bool operator!=(unsigned int __end) const {
            return _M_edge_idx != __end;
            //return __end != __End_Sentiel;
        }
        const __edge& operator*() const{
            return _M_pg->edges[_M_edge_idx];
        }
    private:
        unsigned int _M_edge_idx;
        graph const  * _M_pg;

    };

    struct Iterator_all {
        using CIter = std::vector<__edge>::const_iterator;
        Iterator_all() = delete;
        Iterator_all(const CIter& iter)
            :iter{iter}
        {};
        bool operator!=(const CIter& __end) const{ return iter != __end; }
        void operator++(){ iter++; }
        void operator++(int){ iter++; }

        auto operator*() ->__edge::__edge_thin
        { return  __edge::get(*iter); }
    private:
        CIter iter;
        graph const  * _M_pg;
    };

    graph() = delete;
    graph(unsigned int point_count,bool directed = false)
        : directed{directed} ,_M_point_count{point_count}
    {
        head.resize(point_count+1,__End_Sentiel); //设置大小，所有的元素为-1
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

    auto begin() const
        -> Iterator_all {
        return Iterator_all(edges.begin());
    }

    //auto begin() {
        //return edges.begin();
    //}

    //auto end(){
        //return  edges.end();
    //}
    auto end() const
        -> Iterator_all::CIter {
        return  edges.end();
    }

    //添加输出功能
    // TODO 有向图 与 无向图的输出不一样
    // 有向图 所有边都输出
    // 无向图 只输出一次边
    friend std::ostream& operator<<(std::ostream& __out,const graph& __tg) {
        if( __tg._M_with_head)
            __out << __tg.point_count() << ' ' << __tg.edges_size() << '\n';
        bool flag = 0;
        for (const auto& e : __tg) {
            auto [u,v,w] = e;
            __out << u << ' ' << v;
            if( __tg._M_with_weight ) __out << ' ' << w;
            __out << '\n';
            if( !__tg.directed && (flag=!flag) ) continue;
        }
        return __out;
    }


    //得到边的数量,这个和有向图与无向图有关
    unsigned int edges_size() const { return directed ? _M_edges_count : (_M_edges_count) / 2; }
    unsigned int point_count() const{ return _M_point_count; }

    //TODO 把回以点u为超点的所有边
    auto edges_by_head(unsigned int u) const {
        return Iterator_for_common_head(u,this);
    }
    
    // static
    template<typename... Args>
    static graph tree(int point_count,Args... args);

    template<typename... Args>
    static graph binary_tree(int point_count,Args... args);
    // static end

private:
    auto __next_edges_idx(unsigned int edges_idx) const
        ->decltype(__End_Sentiel)
    {
        return edges_idx == __End_Sentiel ? __End_Sentiel
                            : edges[edges_idx].nxt;
    }
    void __add_edge(int u,int v,int w=0){
        edges.push_back( {u,v,w,head[u]} );
        head[u] = _M_edges_count++;
    }
    unsigned int _M_point_count;
    unsigned int _M_edges_count{0};
    bool directed = false;
    bool _M_with_weight = true; //输出时带有 weight
    bool _M_with_head   = true; //输出时 先输出 结点数量 与  边数

    //std::vector<__LINE__TYPE> edges;
    std::vector<unsigned int> head;
    std::vector<__edge> edges; //使用vecotor 做为前向星结构
    //std::list<__edge> edges; //使用 链表 做为前向星结构
};

template<typename... Args>
graph graph::tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    //std::pair<unsigned int,unsigned int> _M_weight_limit = std::make_pair(3, 8);

    //bool _L_directed = __pick_or_default<directedType,Args...>(__args_tuple,false);
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
    
    graph __tg(point_count,true); //

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
