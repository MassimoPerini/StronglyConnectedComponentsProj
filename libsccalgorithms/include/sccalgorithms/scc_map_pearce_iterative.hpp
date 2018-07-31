//
// Created by Massimo Perini on 28/07/18.
//

#ifndef SCC_SCC_MAP_PEARCE_ITERATIVE_HPP
#define SCC_SCC_MAP_PEARCE_ITERATIVE_HPP

#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_iterator.hpp>

#include <stack>


//typedef boost::graph_traits<Graph>::adjacency_iterator adj_iterator;

namespace sccalgorithms {

    namespace pearce2_iterative_details {

        //typedef boost::graph_traits<Graph>::adjacency_iterator adj_iterator;

        template <class Graph, class ComponentMap>
        int visit_pearce_loop(
                           unsigned int &traversal_counter,
                           const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                           const Graph &graph,
                           std::vector<int> &r_index,
                           std::vector<bool> &root,
                           std::stack <int> &stack1,
                           int &c,
                           std::stack <typename boost::graph_traits<Graph>::vertex_descriptor > &vS,
                           std::stack <std::pair <typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator > > &iS,
                           typename boost::property_traits<ComponentMap>::value_type &num_sccs,
                           ComponentMap components)
        {
            typename boost::graph_traits<Graph>::vertex_descriptor v = vS.top();
            std::pair <typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator > info;
            int vertex_index = boost::get(vertex_index_map, v);
            while (iS.top().first != iS.top().second) //I need to check a subgraph
            {
                typename boost::graph_traits<Graph>::vertex_descriptor new_node = (*(iS.top().first));
                int index = boost::get(vertex_index_map, new_node);
                // avoid the first time of each node (the first child is not visited). THE SAME node has already been visited
                if (r_index[index] >= 1) {

                    if (r_index[index] < r_index[vertex_index])
                    {
                        r_index[vertex_index] = r_index[index];
                        root[vertex_index] = false;
                    }
                    //I checked this subgraph
                    //next step with iterator
                    info = iS.top();
                    typename boost::graph_traits<Graph>::adjacency_iterator nxt = boost::next(info.first);
                    info.first = nxt;
                    iS.pop();
                    iS.push(info);
                }

                if (iS.top().first != iS.top().second) {
                    typename boost::graph_traits<Graph>::vertex_descriptor new_node = (*(iS.top().first));
                    int index = boost::get(vertex_index_map, new_node);
                    if (r_index[index] < 1) { //not explored
                        root[index] = true;
                        r_index[index] = traversal_counter;
                        traversal_counter++;
                        vS.push(new_node);
                        iS.push(adjacent_vertices(new_node, graph));
                        return 0;
                    }
                }
            }

            //take decision

            iS.pop();
            vS.pop();
            int count = 0;
            if (root[vertex_index])
            {
                count = 1;
                traversal_counter--;
                //cout<<"NEW COMPONENT FOUND: "<<vertex_index;

                while (!stack1.empty() && r_index[vertex_index] <= r_index[stack1.top()])
                {
                    int index_2 = stack1.top();
                    stack1.pop();
                    boost::put(components, index_2, num_sccs);
                    r_index[index_2] = c;
                    traversal_counter--;
                    //cout<<", "<<index_2;
                }
                boost::put(components, v, num_sccs);
                r_index[vertex_index] = c;
                c--;
                num_sccs++;

            }
            else{
                stack1.push(vertex_index);
            }

            return count;
        }
    };

    template <class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    pearce2_map_iterative_scc(const Graph &graph, ComponentMap components)
    {
        using namespace pearce2_iterative_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;

        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map = boost::get(boost::vertex_index, graph);

        typename boost::property_traits<ComponentMap>::value_type num_sccs = 0;
        size_type l = num_vertices(graph);

        // ---

        //unsigned int num_sccs = 0;
        //unsigned long l= num_vertices(graph);
        std::vector<int>  r_index(l);
        std::vector<bool>  root(l);
        std::stack <int> stack1;
        typename boost::property_traits<ComponentMap>::value_type enumerate_scc = 0;


        unsigned int traversal_counter = 1;
        int c = l-1;

        std::stack <typename boost::graph_traits<Graph>::vertex_descriptor> vS;
        std::stack <std::pair <typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator > > iS;

        for (int i = 0;i<l;i++)
        {
            r_index[i] = 0;
        }

        BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            int int_value = boost::get(vertex_index_map, v);

            if (r_index[int_value] == 0)
            {
                vS.push(v);
                iS.push(adjacent_vertices(v, graph));

                root[int_value] = true;
                r_index[int_value] = traversal_counter;
                traversal_counter++;


                while(!vS.empty()) {
                    num_sccs+=visit_pearce_loop(traversal_counter, vertex_index_map, graph, r_index, root, stack1, c, vS, iS, enumerate_scc, components);
                }
            }
        }

        return num_sccs;

    }

};

#endif //SCC_SCC_MAP_PEARCE_ITERATIVE_HPP
