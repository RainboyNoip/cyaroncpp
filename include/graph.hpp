#pragma once
#ifndef ___CYARON_SINGLE_HPP__
#include "core/base.hpp"
#include "core/traits.hpp"
#include "random.hpp"
#endif


namespace cyaron {

MAKE__NamedType(chain,double)
MAKE__NamedType(flower,double)
MAKE__NamedType(left,double)
MAKE__NamedType(right,double)
MAKE__NamedType(directed,bool)
MAKE__NamedType(self_loop,bool)
MAKE__NamedType(repeated_edges,bool)
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

struct Graph {

private:

//using __LINE__TYPE =  std::vector<__edge> ;
using __Storage_edge_type = std::list<__edge>;

public:
    
    struct Iterator_for_common_head {
        Iterator_for_common_head() = delete;
        explicit
        Iterator_for_common_head(const unsigned int head_num,Graph const * THIS_pg):
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
        Graph const  * _M_pg;

    };

    struct Iterator_all {
        using CIter = std::vector<__edge>::const_iterator;
        Iterator_all() = delete;
        Iterator_all(const CIter& iter, Graph const * __graph )
            :iter{iter},_M_pg{__graph}
        {};
        bool operator!=(const CIter& __end) const{ return iter != __end; }
        void operator++(){ ++iter; 
            if( !_M_pg->directed && *this != _M_pg->end())
                ++iter;
        }
        void operator++(int){ ++*this; }

        auto operator*() ->__edge::__edge_thin
        { return  __edge::get(*iter); }
    private:
        CIter iter;
        Graph const  * _M_pg;
    };

    Graph() = delete;
    Graph(unsigned int point_count,bool directed = false)
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
        return Iterator_all(edges.begin(),this);
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
    friend std::ostream& operator<<(std::ostream& __out,const Graph& __tg) {
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
    unsigned int edges_size() const { return directed ? _M_edges_count : (_M_edges_count) / 2; }
    unsigned int point_count() const{ return _M_point_count; }

    //TODO 把回以点u为超点的所有边
    auto edges_by_head(unsigned int u) const {
        return Iterator_for_common_head(u,this);
    }
    
    // static
    template<typename... Args>
    static Graph tree(int point_count,Args... args);

    template<typename... Args>
    static Graph binary_tree(int point_count,Args... args);

    template<typename... Args>
    static Graph graph(int point_count,int edge_count,Args... args);

    //保证连通的图,且限定edge_count的范围
    template<typename... Args>
    static Graph graph_connected(int point_count,int edge_count,Args... args);
    // static end
    //
    void set_with_head(bool with = false){ _M_with_head = with; }
    void set_with_weight(bool with = false){_M_with_weight = with;}

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
Graph Graph::tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    //std::pair<unsigned int,unsigned int> _M_weight_limit = std::make_pair(3, 8);

    //bool _L_directed = __pick_or_default<directedType,Args...>(__args_tuple,false);
    //bool _L_directed = ___pick_or_default<directedType>(__args_tuple,false);
    double _L_chain  = __pick_or_default<chainType>(__args_tuple,0.0);
    double _L_flower = __pick_or_default<flowerType>(__args_tuple,0.0);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(1,1));

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
    
    Graph __tg(point_count,true); //

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

template<typename... Args>
Graph Graph::binary_tree(int point_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    double _L_left  = __pick_or_default<leftType>(__args_tuple,0.0);
    double _L_right = __pick_or_default<rightType>(__args_tuple,0.0);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(1,1));

    if( _L_left < 0 || _L_left > 1 || _L_right < 0 || _L_right > 1){
        throw  std::invalid_argument("left and right must be between 0 and 1");
    }
    if ( _L_left + _L_right > 1)
        throw  std::invalid_argument("left plus right must be smaller than 1");
    
    std::set<unsigned int> can_left{1};
    std::set<unsigned int> can_right{1};

    Graph __tg(point_count,true); //
    for(int i=2;i<=point_count;++i){
        double edge_pos = RND.random();
        //left
        int node = 0;
        if( edge_pos < _L_left || 
            ( _L_left + _L_right < edge_pos  && edge_pos <= (1.0-_L_left - _L_right)/2 ) 
          ){ 
            auto it = can_left.begin();
            std::advance(it,RND(0,can_left.size()-1));
            node = *it;
            can_left.erase(it);
        }
        else { // right
            auto it = can_right.begin();
            std::advance(it,RND(0,can_right.size()-1));
            node = *it;
            can_right.erase(it);
        }
        __tg.add_edge(node, i);
        can_left.insert(i);
        can_right.insert(i);
    }
    return __tg;
}

template<typename... Args>
Graph Graph::graph(int point_count,int edge_count,Args... args){
    if(point_count <=0)
        throw  std::invalid_argument("point_count must greater than 0!");
    
    auto __args_tuple = std::make_tuple(std::forward<Args>(args)...);

    bool _L_directed = __pick_or_default<directedType>(__args_tuple,false);
    bool _L_self_loop = __pick_or_default<self_loopType>(__args_tuple,true);
    bool _L_repeated_edges = __pick_or_default<repeated_edgesType>(__args_tuple,true);
    std::pair<int, int> _L_weight_limit
        = __pick_or_default<weight_limitType>(__args_tuple,std::make_pair(3,8));

    Graph __tg(point_count,_L_directed);
    std::map<std::pair<int, int>,bool> used_edges;

    int i = 0;
    while ( i < edge_count ) {
        int u = RND(1,point_count);
        int v = RND(1,point_count);

        if ( ( !_L_self_loop && u == v)  
                || ( !_L_repeated_edges  && used_edges[std::make_pair(u, v)] == 1) )
            continue;

        __tg.add_edge(u, v,RND(_L_weight_limit.first,_L_weight_limit.second));

        if( !_L_repeated_edges){
            used_edges[std::make_pair(u, v)] = 1;
            if( !_L_directed )
                used_edges[std::make_pair(v, u)] = 1;
        }
        ++i;
    }
    return __tg;

}

} //namespace cyaron 
