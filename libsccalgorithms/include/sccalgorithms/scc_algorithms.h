#ifndef SCC_SCC_ALGORITHMS_H
#define SCC_SCC_ALGORITHMS_H

#include "sccalgorithms/scc_utils.h"

//typedef typename boost::property_map<vertex_descriptor, boost::vertex_index_t> IndexMap;

typedef typename boost::property_map<sccalgorithms::DirectedGraph, boost::vertex_index_t>::const_type IndexMap;


//typedef boost::property_map<vertex_descriptor, boost::vertex_index_t>::type VertexIndexMap;


namespace sccalgorithms {

    unsigned strong_connected_components(const DirectedGraph &, const Vertex&);
    unsigned int tarjan_ssc(const DirectedGraph &);
    unsigned int nuutila1_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    unsigned int nuutila1_ssc(const sccalgorithms::DirectedGraph &graph);

    unsigned int nuutila2_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    unsigned int nuutila2_ssc(const sccalgorithms::DirectedGraph &graph);

    unsigned int pearce1_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    unsigned int pearce1_ssc(const sccalgorithms::DirectedGraph &graph);

    unsigned int pearce2_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map);
    unsigned int pearce2_ssc(const sccalgorithms::DirectedGraph &graph);
}

#endif //SCC_SCC_ALGORITHMS_H
