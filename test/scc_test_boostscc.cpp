#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> DirectedGraph;

TEST(BoostStrongConnectedComponents, TarjanExample) {
    DirectedGraph graph;

    boost::add_edge(0, 1, graph);
    boost::add_edge(1, 2, graph);
    boost::add_edge(1, 7, graph);
    boost::add_edge(2, 3, graph);
    boost::add_edge(2, 6, graph);
    boost::add_edge(3, 4, graph);
    boost::add_edge(4, 2, graph);
    boost::add_edge(4, 5, graph);
    boost::add_edge(6, 3, graph);
    boost::add_edge(6, 5, graph);
    boost::add_edge(7, 0, graph);
    boost::add_edge(7, 6, graph);

    std::vector<int> component(boost::num_vertices(graph));
    int num_components = boost::strong_components(graph,
            boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, graph)));

    ASSERT_EQ(3, num_components);
}

