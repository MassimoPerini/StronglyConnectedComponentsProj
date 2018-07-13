#ifndef SCC_SCC_ALGORITHMS_H
#define SCC_SCC_ALGORITHMS_H

#include "sccalgorithms/scc_utils.h"
#include <boost/property_map/property_map.hpp>

//typedef typename boost::property_map<vertex_descriptor, boost::vertex_index_t> IndexMap;

typedef typename boost::property_map<sccalgorithms::DirectedGraph, boost::vertex_index_t>::const_type IndexMap;


//typedef boost::property_map<vertex_descriptor, boost::vertex_index_t>::type VertexIndexMap;


namespace sccalgorithms {

    unsigned int tarjan_ssc(const sccalgorithms::DirectedGraph &, IndexMap);
    inline unsigned int tarjan_scc(const sccalgorithms::DirectedGraph & g)  {
        return sccalgorithms::tarjan_ssc(g, boost::get(boost::vertex_index, g));
    }

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
