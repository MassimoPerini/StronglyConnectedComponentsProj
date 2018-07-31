#ifndef SCC_SCC_UTILS_H
#define SCC_SCC_UTILS_H

#include <boost/graph/adjacency_list.hpp>

namespace sccalgorithms {
    /**
     * Base type for graph
     */
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> DirectedGraph;

    /**
     * Vertex type
     */
     typedef boost::graph_traits<DirectedGraph>::vertex_descriptor Vertex;

}

#endif //SCC_SCC_UTILS_H
