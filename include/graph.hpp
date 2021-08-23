#ifndef ___CYARON_SINGLE_HPP__
#include "base.hpp"
#include "traits.hpp"
#include "random.hpp"
#endif

namespace cyaron {

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
        explicit Iterator(unsigned int head_line,const __LINE__TYPE::iterator& iter,graph* father) 
            : head_line{head_line},iter{iter},father{father}
        {}

        void operator++(){
            if( iter == father->edges[father->point_count].end() ) return;
            ++iter;
            if( iter == father->edges[head_line].end()){ //是这一行的结尾
                if( head_line != father->point_count) { // 不是最后一行
                    do {
                        //下一行的开头
                        iter = father->edges[++head_line].begin();
                    }while( 
                            head_line != father->point_count 
                            && iter == father->edges[head_line].end() 
                        );
                }
            }
        }
        void operator++(int){ ++*this; }

        __LINE__TYPE::value_type& operator*(){
            return *iter;
        }

        friend bool operator==(const Iterator & __a,const Iterator & __b){
            return !operator==(__a,__b);
        }
        friend bool operator!=(const Iterator & __a,const Iterator & __b){
            return __a.iter != __b.iter;
        }
        private:
            graph* father;
            unsigned int head_line; //哪个点开头的
            __LINE__TYPE::iterator iter;
    };

    graph() = delete;
    graph(unsigned int point_count,bool directed = false)
        : directed{directed} ,point_count{point_count}
    {
        edges.resize(point_count+1);
    }

    // 转成dot 格式
    std::string to_dot(){
        return "";
    }

    void add_edge(int u,int v,int w = 0){
        __add_edge(u,v,w);
        if( !directed && u != v) 
            __add_edge(v,u,w);
    }

    Iterator begin(){
        for (int i = 1;i<=point_count;++i) {
            auto &v = edges[i];
            if(v.begin() != v.end())
                return Iterator(i,v.begin(),this);
        }
        return end();
    }

    Iterator end(){
        return Iterator(point_count, edges[point_count].end(),this);
    }

    //得到边的数量
    auto edges_size(){ return edges_count; }

    //把回以点u为超点的所有边
    const __LINE__TYPE & edges_by_head(unsigned int u) const{
        return edges[u];
    }

private:
    void __add_edge(int u,int v,int w=0){
        ++edges_count;
        edges[u].push_back( {u,v,w} );
    }
    unsigned int point_count;
    unsigned int edges_count{0};
    bool directed = false;

    std::vector<__LINE__TYPE> edges;
};

} //namespace cyaron 
