//
// Created by Massimo Perini on 25/07/18.
//

#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>


#include <iostream>
#include <stack>

#ifndef SCC_SCC_MAP_ALGORITHMS_H
#define SCC_SCC_MAP_ALGORITHMS_H

using namespace std;
using namespace boost;

namespace sccalgorithms {

    template<class WritableMap>
    inline unsigned int nuutila1_visit(const sccalgorithms::Vertex &node,
                                       int &depth,
                                       const IndexMap &vertex_index_map,
                                       const sccalgorithms::DirectedGraph &graph,
                                       vector<unsigned int> &visitation_index,
                                       vector<int> &root,
                                       vector<bool> &index_components,
                                       stack<int> &stack,
                                       unsigned int &total_scc,
                                       WritableMap map) {
        unsigned int num_sccs = 0;
        int index = boost::get(vertex_index_map, node);
        root[index] = index;
        index_components[index] = false;
        visitation_index[index] = depth;

        for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
        {
            int index_2 = boost::get(vertex_index_map, vd);
            if (visitation_index[index_2] < 1)//not visited -> explore
            {
                depth++;
                num_sccs += sccalgorithms::nuutila1_visit(vd, depth, vertex_index_map, graph, visitation_index, root, index_components,
                                           stack, total_scc, map);
            }

            if (!index_components[index_2]) {
                if (visitation_index[root[index_2]] < visitation_index[root[index]])
                    root[index] = root[index_2];
            }
        }

        if (root[index] == index) {
            num_sccs++;
            //cout<<"NEW COMPONENT FOUND: "<<index;
            index_components[index] = true;

            total_scc++;
            boost::put(map, index, total_scc);

            if (!stack.empty()) {
                int old_index = stack.top();
                while (visitation_index[old_index] > visitation_index[index]) {
                    //cout << " , " << old_index;
                    index_components[old_index] = true;
                    stack.pop();

                    boost::put(map, old_index, total_scc);

                    if (stack.empty()) {
                        break;
                    }
                    old_index = stack.top();
                }
            }
            //cout<<endl;
        } else {
            stack.push(index);  //oppure pushare il nodo direttamente e poi prendere al pop il suo index
        }
        return num_sccs;
    }

    template<class WritableMap>
    inline unsigned int nuutila1_scc_test(const sccalgorithms::DirectedGraph &graph, WritableMap map) {

        unsigned int num_sccs = 0;
        unsigned long l = num_vertices(graph);
        std::vector<int> roots(l);
        std::vector<unsigned int> visitation_indexes(l);
        std::vector<bool> index_components(l);
        std::stack<int> stack;
        int depth = 1;
        IndexMap vertex_index_map = boost::get(boost::vertex_index, graph);
        unsigned int total_scc = 0;

        for (int i = 0; i < l; i++) {
            roots[i] = 0;
            index_components[i] = false;
            visitation_indexes[i] = 0;
        }

        BGL_FORALL_VERTICES(v, graph, DirectedGraph) {
                //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
                int int_value = boost::get(vertex_index_map, v);
                if (visitation_indexes[int_value] == 0) {
                    num_sccs += sccalgorithms::nuutila1_visit(v, depth, vertex_index_map, graph, visitation_indexes, roots, index_components, stack, total_scc, map);
                }
            }

        return num_sccs;

    }

}
#endif //SCC_SCC_MAP_ALGORITHMS_H
