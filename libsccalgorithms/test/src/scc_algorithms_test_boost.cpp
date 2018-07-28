#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <vector>
#include "sccalgorithms/scc_algorithms.h"
#include <iostream>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

#include "sccalgorithms/scc_map_algorithms.hpp"
#include "sccalgorithms/scc_map_tarjan.hpp"
#include "sccalgorithms/scc_map_pearce.hpp"
#include "sccalgorithms/scc_map_pearce_iterative.hpp"
#include "sccalgorithms/scc_map_nuutila_iterative.hpp"


/**
 * Test fixture containing example graphs
 */
class ExampleGraphs : public ::testing::Test {
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
}

TEST_F(ExampleGraphs, MyStrongComponents) {
    ASSERT_EQ(3, sccalgorithms::tarjan_scc(tarjanGraph));
}


TEST_F(ExampleGraphs, Nutilia1) {
    //std::vector<unsigned int> lowpt(num_vertices(tarjanGraph));

    ASSERT_EQ(3, sccalgorithms::nuutila1_scc(tarjanGraph/*, boost::make_iterator_property_map(lowpt.begin(), boost::get(boost::vertex_index, tarjanGraph))*/));
}

TEST_F(ExampleGraphs, Nutilia1Test) {
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::nuutila1_scc_test(tarjanGraph,
                                                          boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i)
        ASSERT_EQ(component_boost[i], component[i]);
}

TEST_F(ExampleGraphs, Nutilia2) {
    ASSERT_EQ(3, sccalgorithms::nuutila2_scc(tarjanGraph));
}

TEST_F(ExampleGraphs, Pearce1) {
    ASSERT_EQ(3, sccalgorithms::pearce1_scc(tarjanGraph));
}

TEST_F(ExampleGraphs, Pearce2) {
    ASSERT_EQ(3, sccalgorithms::pearce2_scc(tarjanGraph));
}

TEST_F(ExampleGraphs, Nuutila1Iterative){
    ASSERT_EQ(3, sccalgorithms::nuutila1_iterative_ssc(tarjanGraph));
}

TEST_F(ExampleGraphs, Nuutila2Iterative){
    ASSERT_EQ(3, sccalgorithms::nuutila2_iterative_ssc(tarjanGraph));
}

TEST_F(ExampleGraphs, Pearce2Iterative){
    ASSERT_EQ(3, sccalgorithms::pearce2_iterative_ssc(tarjanGraph));
}

TEST_F(ExampleGraphs, TarjanMap){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::tarjan_map_scc(tarjanGraph,
                                                  boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                  boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i)
        ASSERT_EQ(component_boost[i], component[i]);
}

TEST_F(ExampleGraphs, Pearce1Map){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::pearce1_map_scc(tarjanGraph,
                                                       boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i)
        ASSERT_EQ(component_boost[i], component[i]);
}

TEST_F(ExampleGraphs, Pearce2Map){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::pearce2_map_scc(tarjanGraph,
                                                       boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i) {
        //std::cout<<component_boost[i]<<" ";
        ASSERT_EQ(component_boost[i], component[i]);
    }
    //std::cout<<std::endl;
}

TEST_F(ExampleGraphs, Pearce2MapIterative){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::pearce2_map_iterative_scc(tarjanGraph,
                                                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i) {
        //std::cout<<component[i]<<" "<<std::endl;
        ASSERT_EQ(component_boost[i], component[i]);
    }
    //std::cout<<std::endl;

}

TEST_F(ExampleGraphs, Nuutila1MapIterative){
    std::vector<int> component(boost::num_vertices(tarjanGraph));
    std::vector<int> component_boost(boost::num_vertices(tarjanGraph));

    int num_components = sccalgorithms::nuutila1_map_iterative_scc(tarjanGraph,
                                                                  boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, tarjanGraph)));
    int num_components_boost = boost::strong_components(tarjanGraph,
                                                        boost::make_iterator_property_map(component_boost.begin(), boost::get(boost::vertex_index, tarjanGraph)));

    ASSERT_EQ(num_components_boost, num_components);
    for (int i=0; i<component.size(); ++i) {
        //std::cout<<component[i]<<" "<<std::endl;
        ASSERT_EQ(component_boost[i], component[i]);
    }
    //std::cout<<std::endl;

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