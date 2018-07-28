#ifndef SCC_SCC_ALGORITHMS_H
#define SCC_SCC_ALGORITHMS_H

#include <boost/graph/strong_components.hpp>
#include <boost/property_map/property_map.hpp>
#include <vector>
#include <functional>
#include "sccalgorithms/scc_utils.h"
#include "sccalgorithms/scc_algorithm.h"
#include "sccalgorithms/scc_map_tarjan.hpp"
#include "sccalgorithms/scc_map_nuutila.hpp"
#include "sccalgorithms/scc_map_nuutila_iterative.hpp"
#include "sccalgorithms/scc_map_pearce.hpp"
#include "sccalgorithms/scc_map_pearce_iterative.hpp"

typedef typename boost::property_map<sccalgorithms::DirectedGraph, boost::vertex_index_t>::const_type IndexMap;


namespace sccalgorithms {

    unsigned int tarjan_scc(const sccalgorithms::DirectedGraph &, IndexMap);
    inline unsigned int tarjan_scc(const sccalgorithms::DirectedGraph & g)  {
        return sccalgorithms::tarjan_scc(g, boost::get(boost::vertex_index, g));
    }

    unsigned int nuutila1_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int nuutila1_scc(const sccalgorithms::DirectedGraph &graph){
        return sccalgorithms::nuutila1_scc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int nuutila2_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int nuutila2_scc(const sccalgorithms::DirectedGraph &graph){
        return sccalgorithms::nuutila2_scc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int pearce1_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int pearce1_scc(const sccalgorithms::DirectedGraph &graph){
        return sccalgorithms::pearce1_scc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int pearce2_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int pearce2_scc(const sccalgorithms::DirectedGraph &graph){
        return sccalgorithms::pearce2_scc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int nuutila1_iterative_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int nuutila1_iterative_ssc(const sccalgorithms::DirectedGraph &graph) {
        return sccalgorithms::nuutila1_iterative_ssc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int nuutila2_iterative_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int nuutila2_iterative_ssc(const sccalgorithms::DirectedGraph &graph) {
        return sccalgorithms::nuutila1_iterative_ssc(graph, boost::get(boost::vertex_index, graph));
    }

    unsigned int pearce2_iterative_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    inline unsigned int pearce2_iterative_ssc(const sccalgorithms::DirectedGraph &graph) {
        return sccalgorithms::pearce2_iterative_ssc(graph, boost::get(boost::vertex_index, graph));
    }

    template <class Graph, class ComponentMap = typename boost::iterator_property_map<typename std::vector<int>::iterator, typename boost::property_map<Graph, boost::vertex_index_t>::const_type> >    inline
    const std::vector<sccalgorithms::scc_algorithm<Graph, ComponentMap> > availableAlgorithms() {
        return {
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("boost", [](const Graph & g, ComponentMap components){return boost::strong_components(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("tarjan_map", [](const Graph & g, ComponentMap components){return sccalgorithms::tarjan_map_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("nuutila1_map", [](const Graph & g, ComponentMap components){return sccalgorithms::nuutila1_map_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("nuutila1_iterative", [](const Graph & g, ComponentMap components){return sccalgorithms::nuutila1_map_iterative_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("nuutila2_map", [](const Graph & g, ComponentMap components){return sccalgorithms::nuutila2_map_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("nuutila2_iterative", [](const Graph & g, ComponentMap components){return sccalgorithms::nuutila2_map_iterative_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("pearce1_map", [](const Graph & g, ComponentMap components){return sccalgorithms::pearce1_map_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("pearce2_map", [](const Graph & g, ComponentMap components){return sccalgorithms::pearce2_map_scc(g, components);}),
                sccalgorithms::scc_algorithm<Graph, ComponentMap>("pearce2_iterative", [](const Graph & g, ComponentMap components){return sccalgorithms::pearce2_map_iterative_scc(g, components);}),
        };
    };
}

#endif //SCC_SCC_ALGORITHMS_H
