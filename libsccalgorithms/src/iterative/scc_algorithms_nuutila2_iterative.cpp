//
// Created by Massimo Perini on 22/07/18.
//

//
// Created by Massimo Perini on 21/07/18.
//

#include "sccalgorithms/scc_algorithms.h"

#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>



#include <stack>
#include <iostream>

using namespace std;
using namespace boost;

typedef graph_traits <sccalgorithms::DirectedGraph>::adjacency_iterator adj_iterator;


int visit_loop(const sccalgorithms::Vertex &node,
               unsigned int &traversal_counter,
               const IndexMap &vertex_index_map,
               const sccalgorithms::DirectedGraph &graph,
               vector<unsigned int> &visitation_index,
               vector<int> &root,
               vector<bool> &index_components,
               stack <int> &stack1,
               vector<bool> &is_on_stack,
               stack<sccalgorithms::Vertex> &vS,
               stack <pair <adj_iterator, adj_iterator > >&iS)

{
    sccalgorithms::Vertex v = vS.top();
    pair <adj_iterator, adj_iterator > info;

    int vertex_index = boost::get(vertex_index_map, v);

    while (iS.top().first != iS.top().second) //I need to check a subgraph
    {
        sccalgorithms::Vertex new_node = (*(iS.top().first));
        int index = boost::get(vertex_index_map, new_node);

        // avoid the first time of each node (the first child is not visited). THE SAME node has already been visited
        if (visitation_index[index] >= 1) {
            if (!index_components[root[index]]) {
                if (visitation_index[root[index]] < visitation_index[root[vertex_index]])
                    root[vertex_index] = root[index];
            }

            //I checked this subgraph
            //next step with iterator
            info = iS.top();
            adj_iterator nxt = boost::next(info.first);
            info.first = nxt;
            iS.pop();
            iS.push(info);

        }

        if (iS.top().first != iS.top().second) {

            sccalgorithms::Vertex new_node = (*(iS.top().first));
            int index = boost::get(vertex_index_map, new_node);

            if (visitation_index[index] < 1) { //not explored
                root[index] = index;
                index_components[index] = false;
                visitation_index[index] = traversal_counter;
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

    if (root[vertex_index] == vertex_index)
    {
        count = 1;
        //cout<<"NEW COMPONENT FOUND: "<<vertex_index;
        if (!stack1.empty() && visitation_index[stack1.top()] >= visitation_index[vertex_index]) {
            while (!stack1.empty() && visitation_index[stack1.top()] >= visitation_index[vertex_index]) {
                index_components[stack1.top()] = true;
                is_on_stack[stack1.top()] = false;
                stack1.pop();
            }
        } else {
            index_components[vertex_index] = true;
        }
    }
    else if (!is_on_stack[root[vertex_index]]) {
        stack1.push(root[vertex_index]);
        is_on_stack[root[vertex_index]] = true;
    }

    return count;
}


unsigned int sccalgorithms::nuutila2_iterative_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{
    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);
    std::vector<int> roots(l);
    std::vector<unsigned int> visitation_index(l);
    std::vector<bool> index_components(l);
    std::vector<bool> is_on_stack(l);


    unsigned int traversal_counter = 1;

    std::stack <int> stack;
    std::stack <sccalgorithms::Vertex> vS;
    std::stack <pair <adj_iterator, adj_iterator > > iS;

    for (int i = 0;i<l;i++)
    {
        roots[i] = 0;
        index_components[i] = false;
        visitation_index[i] = 0;
        is_on_stack[i] = false;
    }

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            int int_value = boost::get(vertex_index_map, v);
            if (visitation_index[int_value] == 0)
            {
                vS.push(v);
                iS.push(adjacent_vertices(v, graph));

                index_components[int_value] = false;
                roots[int_value] = int_value;
                visitation_index[int_value] = traversal_counter;
                traversal_counter ++;

                while(!vS.empty()) {
                    num_sccs+=visit_loop(v, traversal_counter, vertex_index_map, graph, visitation_index, roots, index_components, stack, is_on_stack, vS, iS);
                }
            }
        }

    return num_sccs;
}

