#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <vector>
#include "sccalgorithms/scc_algorithms.h"

#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

/**
 * Test fixture containing example graphs
 */
class ExampleGraphs : public ::testing::Test {
public:
    inline bool checkRightComponents(const std::vector<int> & testVector) {
        return testVector == right_components;
    }
private:
    const std::vector<int> right_components{2, 2, 1, 1, 1, 0, 1, 2};
protected:
    void SetUp() override {
        boost::add_edge(0, 1, tarjanGraph);
        boost::add_edge(1, 2, tarjanGraph);
        boost::add_edge(1, 7, tarjanGraph);
        boost::add_edge(2, 3, tarjanGraph);
        boost::add_edge(2, 6, tarjanGraph);
        boost::add_edge(3, 4, tarjanGraph);
        boost::add_edge(4, 2, tarjanGraph);
        boost::add_edge(4, 5, tarjanGraph);
        boost::add_edge(6, 3, tarjanGraph);
        boost::add_edge(6, 5, tarjanGraph);
        boost::add_edge(7, 0, tarjanGraph);
        boost::add_edge(7, 6, tarjanGraph);
    }

    sccalgorithms::DirectedGraph tarjanGraph;
};

TEST_F(ExampleGraphs, BoostStrongComponents) {
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    int num_components = boost::strong_components(tarjanGraph,
            boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, TarjanMap){
    std::vector<int> component(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::tarjan_map_scc(tarjanGraph,
                                                  boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, Nutilia1Map) {
    std::vector<int> component(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::nuutila1_map_scc(tarjanGraph,
                                                          boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, Nutilia2Map) {
    std::vector<int> component(boost::num_vertices(tarjanGraph), -1);
    std::vector<int> expected{2, -1, 1, -1, -1, 0, -1, -1};
    int num_components = sccalgorithms::nuutila2_map_scc(tarjanGraph,
                                                          boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(component == expected);
}

TEST_F(ExampleGraphs, Pearce1Map){
    std::vector<int> component(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::pearce1_map_scc(tarjanGraph,
                                                       boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, Pearce2Map){
    std::vector<int> component(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::pearce2_map_scc(tarjanGraph,
                                                       boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, Pearce2MapIterative){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    int num_components = sccalgorithms::pearce2_map_iterative_scc(tarjanGraph,
                                                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));

}

TEST_F(ExampleGraphs, Nuutila1MapIterative){
    std::vector<int> component(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::nuutila1_map_iterative_scc(tarjanGraph,
                                                                  boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    ASSERT_EQ(3, num_components);
    ASSERT_TRUE(checkRightComponents(component));
}

TEST_F(ExampleGraphs, Nuutila2MapIterative){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::nuutila2_map_iterative_scc(tarjanGraph,
                                                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    int count_child = 0;
    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i) {
        if (component[i] != 0)
        {
            ASSERT_EQ(component_boost[i]+1, component[i]);
        }
        else {
            count_child++;
        }
    }
    ASSERT_GE(count_child, num_components_boost);
}