#ifndef SCC_SCC_MAP_PEARCE_HPP
#define SCC_SCC_MAP_PEARCE_HPP

#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>

#include <stack>

namespace sccalgorithms {

    namespace pearce1_details {


        template <class Graph, class ComponentMap>
        void pearce1_visit(const typename boost::graph_traits<Graph>::vertex_descriptor &node,
                           int &depth,
                           const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                           const Graph &graph,
                           std::vector<int> &r_index,
                           std::vector<bool> &index_components,
                           std::vector<bool> &visited,
                           std::stack <int> &stackVertices,
                           int &c,
                           typename boost::property_traits<ComponentMap>::value_type &num_sccs,
                           ComponentMap components)
        {
            int index = boost::get(vertex_index_map, node);
            bool root = true;
            visited[index] = true;
            r_index[index] = depth;

            for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
            {
                int index_2 = boost::get(vertex_index_map,vd);
                if (!visited[index_2])//not visited -> explore
                {
                    depth++;
                    pearce1_visit(vd, depth, vertex_index_map, graph, r_index, index_components, visited, stackVertices,
                                  c, num_sccs, components);
                }

                if (!index_components[index_2] && r_index[index_2] < r_index[index])
                {
                    r_index[index] = r_index[index_2];
                    root = false;
                }
            }

            if (root) {
                index_components[index] = true;
                while (!stackVertices.empty() && r_index[index] <= r_index[stackVertices.top()])
                {
                    int index_2 = stackVertices.top();
                    stackVertices.pop();
                    r_index[index_2] = c;
                    index_components[index_2] = true;
                    boost::put(components, index_2, num_sccs);
                }
                r_index[index] = c;
                boost::put(components, node, num_sccs);
                c++;
                num_sccs++;
            }
            else {
                stackVertices.push(index);
            }
        } // pearce1_visit();

    }; // namespace pearce_details;


    template <class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    pearce1_map_scc(const Graph &graph, ComponentMap components)
    {
        using namespace pearce1_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;


        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map =
                boost::get(boost::vertex_index, graph);

        typename boost::property_traits<ComponentMap>::value_type num_sccs = 0;
        size_type l = num_vertices(graph);

        std::vector<int>  r_index(l, 0);
        std::vector<bool> index_components(l, false);
        std::vector<bool> visited(l, false);
        std::stack <int> stackVertices;
        int depth = 1;
        int c = 0;

        BGL_FORALL_VERTICES_T(v, graph, Graph) {
                int int_value = boost::get(vertex_index_map,v);
                if (!visited[int_value]) {
                    pearce1_visit(v, depth, vertex_index_map, graph, r_index, index_components, visited, stackVertices,
                                  c, num_sccs, components);
                }
            }

        return num_sccs;
    } // pearce1_scc();


    namespace pearce2_details {

        template<class Graph, class ComponentMap>
        void pearce2_visit(const typename boost::graph_traits<Graph>::vertex_descriptor &node,
                           const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                           const Graph &graph,
                           std::vector<int> &r_index,
                           std::stack<int> &stackVertices,
                           int &index_component,
                           int &c,
                           typename boost::property_traits<ComponentMap>::value_type &num_sccs,
                           ComponentMap components) {
            int index = boost::get(vertex_index_map, node);
            bool root = true;
            r_index[index] = index_component;
            index_component++;

            for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
            {
                int index_2 = boost::get(vertex_index_map, vd);

                if (r_index[index_2] == 0) {
                    pearce2_visit(vd, vertex_index_map, graph, r_index, stackVertices, index_component, c, num_sccs, components);
                }
                if (r_index[index_2] < r_index[index]) {
                    r_index[index] = r_index[index_2];
                    root = false;
                }
            }

            if (root) {
                index_component--;
                while (!stackVertices.empty() && r_index[index] <= r_index[stackVertices.top()]) {
                    int index_2 = stackVertices.top();
                    stackVertices.pop();
                    boost::put(components, index_2, num_sccs);
                    r_index[index_2] = c;
                    index_component--;
                }
                boost::put(components, node, num_sccs);
                r_index[index] = c;
                c--;
                num_sccs++;
            } else {
                stackVertices.push(index);
            }
        } // pearce2_visit();

    }; // namespace pearce2_details;

    template <class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    pearce2_map_scc(const Graph &graph, ComponentMap components)
    {
        using namespace pearce2_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;


        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map =
                boost::get(boost::vertex_index, graph);

        typename boost::property_traits<ComponentMap>::value_type num_sccs = 0;
        size_type l = num_vertices(graph);

        std::vector<int> r_index(l, 0);
        std::stack <int> stackVertices;
        int index = 1;
        int c = l-1;

        BGL_FORALL_VERTICES(v, graph, DirectedGraph)
            {
                int int_value = boost::get(vertex_index_map,v);
                if (r_index[int_value] == 0)
                {
                    pearce2_visit(v,vertex_index_map, graph, r_index, stackVertices, index, c, num_sccs, components);
                }
            }

        return num_sccs;
    } // pearce2_scc

}; // namespace sccalgorithms;

#endif //SCC_SCC_MAP_PEARCE_HPP
