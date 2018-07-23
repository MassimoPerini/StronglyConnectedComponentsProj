//
// Created by Massimo Perini on 23/07/18.
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
               vector<int> &r_index,
               vector<bool> &root,
               stack <int> &stack1,
               int &c,
               stack <sccalgorithms::Vertex> &vS,
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
        if (r_index[index] >= 1) {

            if (r_index[index] < r_index[vertex_index])
            {
                r_index[vertex_index] = r_index[index];
                root[vertex_index] = false;
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
            r_index[index_2] = c;
            traversal_counter--;
            //cout<<", "<<index_2;
        }
        r_index[vertex_index] = c;
        c--;

    }
    else{
        stack1.push(vertex_index);
    }

    return count;
}


unsigned int sccalgorithms::pearce2_iterative_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{
    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);
    std::vector<int>  r_index(l);
    std::vector<bool>  root(l);
    std::stack <int> stack1;


    unsigned int traversal_counter = 1;
    int c = l-1;

    std::stack <sccalgorithms::Vertex> vS;
    std::stack <pair <adj_iterator, adj_iterator > > iS;

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
                    num_sccs+=visit_loop(v, traversal_counter, vertex_index_map, graph, r_index,root, stack1, c, vS, iS);
                }
            }
        }

    return num_sccs;
}

