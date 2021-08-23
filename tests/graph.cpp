#include "graph.hpp"


int main(){
    using namespace cyaron;
    graph g(5,1);
    g.add_edge(1, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(1, 5);
    g.add_edge(2, 3);
    std::cout << g.edges_size() << std::endl;

    for (const auto& e : g) {
        auto [x,y,w] = e;
        std::cout << x <<" "<< y << std::endl;
        //std::cout << e << std::endl;
    }
    return 0;
}