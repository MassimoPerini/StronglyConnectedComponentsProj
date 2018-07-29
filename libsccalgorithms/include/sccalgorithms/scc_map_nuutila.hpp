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

namespace sccalgorithms {

    namespace nuutila1_details {

        template<class Graph, class ComponentMap>
        inline void nuutila1_visit(const typename boost::graph_traits<Graph>::vertex_descriptor &node,
                                   int &depth,
                                   const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                                   const Graph &graph,
                                   std::vector<unsigned int> &visitation_index,
                                   std::vector<int> &root,
                                   std::vector<bool> &visited,
                                   std::vector<bool> &index_components,
                                   std::stack<int> &stack,
                                   typename boost::property_traits<ComponentMap>::value_type &total_scc,
                                   ComponentMap components) {
            int index = boost::get(vertex_index_map, node);
            root[index] = index;
            index_components[index] = false; // should be already false!?
            visitation_index[index] = depth;
            visited[index] = true;

            for (auto vd : boost::make_iterator_range(
                    boost::adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
            {
                int index_2 = boost::get(vertex_index_map, vd);
                if (!visited[index_2])//not visited -> explore
                {
                    depth++;
                    nuutila1_visit(vd, depth, vertex_index_map, graph, visitation_index, root, visited, index_components,
                                   stack, total_scc, components);
                }

                if (!index_components[index_2]) {
                    if (visitation_index[root[index_2]] < visitation_index[root[index]])
                        root[index] = root[index_2];
                }
            }

            if (root[index] == index) {
                //cout<<"NEW COMPONENT FOUND: "<<index;
                index_components[index] = true;

                boost::put(components, node, total_scc);

                if (!stack.empty()) {
                    int old_index = stack.top();
                    while (visitation_index[old_index] > visitation_index[index]) {
                        //cout << " , " << old_index;
                        index_components[old_index] = true;
                        stack.pop();

                        boost::put(components, old_index, total_scc);

                        if (stack.empty()) {
                            break;
                        }
                        old_index = stack.top();
                    }
                }

                total_scc++;
                //cout<<endl;
            } else {
                stack.push(index);  //oppure pushare il nodo direttamente e poi prendere al pop il suo index
            }
        }

    }; // namespace nuutila_details

    template<class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    nuutila1_map_scc(const Graph &graph, ComponentMap components) {
        using namespace nuutila1_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;

        size_type l = num_vertices(graph);
        std::vector<int> roots(l, 0);
        std::vector<unsigned int> visitation_indexes(l, 0);
        std::vector<bool> index_components(l, false);
        std::vector<bool> visited(l, false);
        std::stack<int> stack;
        int depth = 0;
        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map =
                boost::get(boost::vertex_index, graph);
        typename boost::property_traits<ComponentMap>::value_type total_scc = 0;

        BGL_FORALL_VERTICES_T(v, graph, Graph) {
            //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
            int int_value = boost::get(vertex_index_map, v);
            if (!visited[int_value]) {
                nuutila1_visit(v, depth, vertex_index_map, graph, visitation_indexes, roots, visited, index_components, stack, total_scc, components);
            }
        }

        return total_scc;
    }



    namespace nuutila2_details {

        template<class Graph, class ComponentMap>
        inline void nuutila2_visit(const typename boost::graph_traits<Graph>::vertex_descriptor &node,
                                   int &depth,
                                   const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                                   const Graph &graph,
                                   std::vector<unsigned int> &visitation_index,
                                   std::vector<int> &root,
                                   std::vector<bool> &visited,
                                   std::vector<bool> &index_components,
                                   std::stack<int> &stack,
                                   std::vector<bool> &is_on_stack,
                                   typename boost::property_traits<ComponentMap>::value_type &total_scc,
                                   ComponentMap components) {
            int index = boost::get(vertex_index_map, node);
            root[index] = index;
            index_components[index] = false; // should be already false!?
            visitation_index[index] = depth;
            visited[index] = true;

            for (auto vd : boost::make_iterator_range(
                    boost::adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
            {
                int index_2 = boost::get(vertex_index_map, vd);
                if (!visited[index_2])//not visited -> explore
                {
                    depth++;
                    nuutila2_visit(vd, depth, vertex_index_map, graph, visitation_index, root, visited, index_components,
                                   stack, is_on_stack, total_scc, components);
                }

                if (!index_components[root[index_2]]) {
                    if (visitation_index[root[index_2]] < visitation_index[root[index]])
                        root[index] = root[index_2];
                }
            }

            if (root[index] == index) {
                boost::put(components, root[index], total_scc);
                //cout << endl << "NEW COMPONENT FOUND: " << index <<" IS THE ROOT (?)"<< endl;
                if (!stack.empty() && visitation_index[stack.top()] >= visitation_index[index]) {
                    while (!stack.empty() && visitation_index[stack.top()] >= visitation_index[index]) {
                        index_components[stack.top()] = true;
                        is_on_stack[stack.top()] = false;
                        stack.pop();
                    }
                } else {
                    index_components[index] = true;
                }

                total_scc++;
            }
            else if (!is_on_stack[root[index]]) {
                stack.push(root[index]);
                is_on_stack[root[index]] = true;
            }
        }

    }; // namespace nuutila2_details

    template<class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    nuutila2_map_scc(const Graph &graph, ComponentMap components) {
        using namespace nuutila2_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;

        size_type l = num_vertices(graph);
        std::vector<int> roots(l, 0);
        std::vector<unsigned int> visitation_indexes(l, 0);
        std::vector<bool> index_components(l, false);
        std::stack<int> stack;
        std::vector<bool> is_on_stack(l, false);
        std::vector<bool> visited(l, false);
        int depth = 0;
        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map =
                boost::get(boost::vertex_index, graph);
        typename boost::property_traits<ComponentMap>::value_type total_scc = 0;

        BGL_FORALL_VERTICES_T(v, graph, Graph) {
            int int_value = boost::get(vertex_index_map, v);
            if (!visited[int_value]) {
                nuutila2_visit(v, depth, vertex_index_map, graph, visitation_indexes, roots, visited, index_components, stack, is_on_stack, total_scc, components);
            }
        }

        return total_scc;
    }

}
#endif //SCC_SCC_MAP_ALGORITHMS_H
