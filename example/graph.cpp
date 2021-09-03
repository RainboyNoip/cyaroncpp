#ifndef LOCALTEST
#include <cyaroncpp/cyaron.hpp>
#else
#include "graph.hpp"
#endif


int main(){
    using namespace cyaron;
    Graph g(5,1);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(1, 5);
    g.add_edge(2, 3);
    std::cout << g.edges_size() << std::endl;
    std::cout << g << std::endl;

    for (const auto& e : g) {
        auto [x,y,w] = e;
        std::cout << x <<" "<< y << std::endl;
        //std::cout << e << std::endl;
    }
    std::cout << "================" << std::endl;
    for(auto it = g.begin(); it != g.end() ; it++){
        auto [x,y,w] = *it;
        std::cout << x <<" "<< y << std::endl;
    }
    std::cout << "====tree========" << std::endl;
    auto mytree = Graph::tree(10,
            //directed = true,
            chain = 0.2,
            flower = 0.3,
            weight_limit = {1,10}
            );
    std::cout << mytree.edges_size() << std::endl;
    std::cout << mytree << std::endl;

    std::cout << "==== vex around point ========" << std::endl;
    for (const auto& e : mytree.edges_by_head(1)) {
        std::cout << e << std::endl;
    }
    //auto t = mytree.edges_by_head(1);
    //auto end = t.end();
    //for( auto it =t.begin() ;  it != end; ++it ){
        //std::cout << 1 << std::endl;
    //}
    std::cout << "====binary_tree========" << std::endl;
    auto btree = Graph::binary_tree(10,
            left = 0.2,
            right= 0.7,
            weight_limit = {1,10}
            );
    std::cout << btree << std::endl;

    std::cout << "====graph========" << std::endl;
    auto mygraph = Graph::graph(10, 14,
            repeated_edges = false
            );
    std::cout << mygraph << std::endl;


    return 0;
}
