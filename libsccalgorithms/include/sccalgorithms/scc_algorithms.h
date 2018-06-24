#ifndef SCC_SCC_ALGORITHMS_H
#define SCC_SCC_ALGORITHMS_H

#include "sccalgorithms/scc_utils.h"

namespace sccalgorithms {

    unsigned strong_connected_components(const DirectedGraph &, const Vertex&);
    unsigned int tarjan_ssc(const DirectedGraph &);
    unsigned int nuutila_ssc(const DirectedGraph &);

}

#endif //SCC_SCC_ALGORITHMS_H
