#include "sccalgorithms/scc_algorithms.h"
#include <vector>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <stack>
#include <numeric>
#include <iostream>

constexpr int NOT_ASSIGNED_NUMBER = 0;

template<class LowptProp, class LowvineProp, class NumberProp, class ContainsVertexProp>
unsigned helper_scc_tarjan(const sccalgorithms::DirectedGraph &, const sccalgorithms::Vertex &, LowptProp, LowvineProp,
                           NumberProp, ContainsVertexProp, std::stack<sccalgorithms::Vertex> &, unsigned &);

template<class LowptProp, class LowvineProp, class NumberProp, class ContainsVertexProp>
unsigned helper_scc_tarjan(const sccalgorithms::DirectedGraph & graph,
                           const sccalgorithms::Vertex & v,
                           LowptProp lowpts,
                           LowvineProp lowvines,
                           NumberProp numbers,
                           ContainsVertexProp is_on_stack,
                           std::stack<sccalgorithms::Vertex> & visited,
                           unsigned& i) {
    boost::put(numbers, v, ++i);
    boost::put(lowpts, v, i);
    boost::put(lowvines, v, i);
    boost::put(is_on_stack, v, true);
    visited.push(v);

    unsigned int num_sccs = 0;

    BGL_FORALL_ADJ(v, w, graph, sccalgorithms::DirectedGraph) {
        if (boost::get(numbers, w) == NOT_ASSIGNED_NUMBER) {
            // (v, w) is a tree arc;
            num_sccs += helper_scc_tarjan(graph, w, lowpts, lowvines, numbers, is_on_stack, visited, i);
            boost::put(lowpts, v,
                       std::min(boost::get(lowpts, v),
                                boost::get(lowpts, w)));
            boost::put(lowvines, v,
                       std::min(boost::get(lowvines, v),
                                boost::get(lowvines, w)));
        } else if (boost::get(lowpts, w) == boost::get(numbers, w) && // v is anchestor of w
                   boost::get(numbers, w) < boost::get(numbers, v) &&
                   boost::get(is_on_stack, w)) {
            boost::put(lowpts, v,
                       std::min(
                               boost::get(lowpts, v),
                               boost::get(numbers, w)));
        } else if (boost::get(numbers, w) < boost::get(numbers, v) &&
                   boost::get(is_on_stack, w)) {
            boost::put(lowvines, v,
                       std::min(
                               boost::get(lowvines, v),
                               boost::get(numbers, w)));
        }
    }

    if(boost::get(lowpts, v) == boost::get(numbers, v) &&
       boost::get(lowvines, v) == boost::get(numbers, v)) {
        // start new connected componet
        while (!visited.empty() && boost::get(numbers, visited.top()) >= boost::get(numbers, v)) {
            boost::put(is_on_stack, visited.top(), false);
            visited.pop();
        }

        num_sccs++;
    }

    return num_sccs;
}

unsigned int sccalgorithms::tarjan_ssc(const DirectedGraph & graph, IndexMap vertex_index_map) {
    unsigned number_of_sccs = 0;

    // LOWPT, LOWVINE exterior pointer properties
    const unsigned long numVertices = boost::num_vertices(graph);
    std::vector<unsigned> lowpt(numVertices);
    std::vector<unsigned> lowvine(numVertices);
    std::vector<unsigned> numbers(numVertices, NOT_ASSIGNED_NUMBER);
    std::vector<bool> is_on_stack(numVertices, false);
    std::stack<Vertex> stackVertexes;
    unsigned i = 0;

    BGL_FORALL_VERTICES(v, graph, sccalgorithms::DirectedGraph)
    {
        int index_vertex = boost::get(vertex_index_map, v);
        if (numbers[index_vertex] == NOT_ASSIGNED_NUMBER)
            number_of_sccs += helper_scc_tarjan(graph, v,
                                                boost::make_iterator_property_map(lowpt.begin(), vertex_index_map),
                                                boost::make_iterator_property_map(lowvine.begin(), vertex_index_map),
                                                boost::make_iterator_property_map(numbers.begin(), vertex_index_map),
                                                boost::make_iterator_property_map(is_on_stack.begin(), vertex_index_map),
                                                stackVertexes, i);
    }

    return number_of_sccs;
}
