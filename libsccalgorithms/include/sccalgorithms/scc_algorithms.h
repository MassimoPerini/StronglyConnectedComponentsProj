#ifndef SCC_SCC_ALGORITHMS_H
#define SCC_SCC_ALGORITHMS_H

#include "sccalgorithms/scc_utils.h"
#include "sccalgorithms/scc_algorithm.h"
#include <boost/property_map/property_map.hpp>
#include <vector>

//typedef typename boost::property_map<vertex_descriptor, boost::vertex_index_t> IndexMap;

typedef typename boost::property_map<sccalgorithms::DirectedGraph, boost::vertex_index_t>::const_type IndexMap;


//typedef boost::property_map<vertex_descriptor, boost::vertex_index_t>::type VertexIndexMap;


namespace sccalgorithms {

    //template<class WritableMap>
    //unsigned int nuutila1_scc_test(const sccalgorithms::DirectedGraph &graph, WritableMap map);

    unsigned int tarjan_scc(const sccalgorithms::DirectedGraph &, IndexMap);
    inline unsigned int tarjan_scc(const sccalgorithms::DirectedGraph & g)  {
        return sccalgorithms::tarjan_scc(g, boost::get(boost::vertex_index, g));
    }

    //template<class WritableMap>

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

    const std::vector<sccalgorithms::scc_algorithm> availableAlgorithms();
}

#endif //SCC_SCC_ALGORITHMS_H
