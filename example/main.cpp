#include <iostream>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/depth_first_search.hpp"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> DirectedGraph;
typedef boost::graph_traits<DirectedGraph>::vertex_descriptor MyVertex;

class MyVisitor : public boost::default_dfs_visitor {
public:
    template<typename Vertex, typename Graph>
    void discover_vertex(Vertex v, const Graph& g) const {
        std::cout << v << std::endl;
    }
};

int main() {
    DirectedGraph g;

    boost::add_edge(0, 1, g);
    boost::add_edge(0, 3, g);
    boost::add_edge(1, 2, g);
    boost::add_edge(3, 2, g);
    boost::add_edge(2, 4, g);
    boost::add_edge(2, 5, g);

    MyVisitor visitor;
    boost::depth_first_search(g, boost::visitor(visitor));

    // boost::write_graphviz(std::cout, g); // print a dot-syntax representation
    return EXIT_SUCCESS;
}