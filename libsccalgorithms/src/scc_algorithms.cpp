#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>

#include <iostream>
#include <stack>
#include <jmorecfg.h>

using vertex_descriptor = boost::graph_traits<sccalgorithms::DirectedGraph>::vertex_descriptor;

unsigned sccalgorithms::strong_connected_components(const sccalgorithms::DirectedGraph &graph,
                                                    const sccalgorithms::Vertex &start_node) {
    return 0;
}


void nuutila_visit(const vertex_descriptor node, const sccalgorithms::DirectedGraph &graph, std::unordered_map<vertex_descriptor, vertex_descriptor> &root, std::unordered_map<vertex_descriptor, bool> &index_components ,std::stack <vertex_descriptor> &stack)
{
    std::cout << "NODO: "<<node<<std::endl;

    root[node] = node;
    index_components[node] = false;

    for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
    {
        std::cout << "vertex " << vd;
        if (root.count(vd) == 0)    //non explored
        {
            nuutila_visit(vd, graph, root, index_components ,stack);
        }
        if (!index_components[vd])   //migliorare efficienza (dovrebbe essere tempo costante)
        {
            root[node] = std::min(root[node], root[vd]);
        }
    }

    if (root[node] == node)
    {
        index_components[node] = true;
        //ecc...
    }


    return;
}

unsigned int sccalgorithms::nuutila_ssc(const sccalgorithms::DirectedGraph &graph)
{
    unsigned long l= num_vertices(graph);
    //long roots [l];
    std::unordered_map<vertex_descriptor, vertex_descriptor> roots;
    std::unordered_map<vertex_descriptor, bool> index_components;
    std::stack <vertex_descriptor> stack;

    roots.reserve(l);
    index_components.reserve(l);

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            if (roots.count(v) == 0) {
                nuutila_visit(v, graph, roots, index_components, stack);
            }
        }

    return 0;
}
unsigned int sccalgorithms::tarjan_ssc(const sccalgorithms::DirectedGraph &)
{
    return 0;
}