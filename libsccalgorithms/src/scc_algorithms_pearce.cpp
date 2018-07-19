#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>


#include <iostream>
#include <stack>
#include <sccalgorithms/scc_utils.h>

//using vertex_descriptor = boost::graph_traits<sccalgorithms::DirectedGraph>::vertex_descriptor;
using namespace boost;
using namespace std;
//typedef boost::property_map<vertex_descriptor, boost::vertex_index_t>::type VertexIndexMap;
//typedef property_map<vertex_descriptor, vertex_index_t> IndexMap;

//using vertex_descriptor = boost::graph_traits<sccalgorithms::DirectedGraph>::vertex_descriptor;



unsigned int pearce1_visit(const sccalgorithms::Vertex &node,
                   int &depth,
                    const IndexMap &vertex_index_map,
                    const sccalgorithms::DirectedGraph &graph,
                    vector<int> &r_index,
                    vector<bool> &index_components,
                    vector<bool> &visited,
                    stack <int> &stack,
                    int &c
                    )
{
    unsigned int num_sccs = 0;
    int index = boost::get(vertex_index_map, node);
    bool root = true;
    visited[index] = true;
    r_index[index] = depth;
    index_components[index] = false;

    for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
    {
        int index_2 = boost::get(vertex_index_map,vd);
        if (!visited[index_2])//not visited -> explore
        {
            depth++;
            num_sccs += pearce1_visit(vd, depth, vertex_index_map, graph, r_index, index_components, visited, stack, c);
        }

        if (!index_components[index_2] && r_index[index_2] < r_index[index])
        {
            r_index[index] = r_index[index_2];
            root = false;
        }
    }

    if (root) {
        num_sccs++;
        index_components[index] = true;
        //cout<<"New component found: "<<node;
        while (!stack.empty() && r_index[index] <= r_index[stack.top()])
        {
            int index_2 = stack.top();
            stack.pop();
            r_index[index_2] = c;
            index_components[index_2] = true;
            //cout<<", "<<index_2;
        }
        r_index[index] = c;
        c++;
        //cout<<endl;
    }
    else {
        stack.push(index);
    }
    return num_sccs;
}


unsigned int sccalgorithms::pearce1_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{

    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);

    std::vector<int>  r_index(l);
    std::vector<bool> index_components(l);
    std::vector<bool> visited(l);
    std::stack <int> stack;
    int depth = 1;

    int c = 0;

    for (int i = 0;i<l;i++)
    {
        visited[i] = false;
        r_index[i] = 0;
        index_components[i] = false;
    }

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
            int int_value = boost::get(vertex_index_map,v);
            if (!visited[int_value])
            {
                num_sccs += pearce1_visit(v, depth,vertex_index_map, graph, r_index, index_components, visited, stack, c);
            }
        }

    return num_sccs;
}



unsigned int pearce2_visit(const sccalgorithms::Vertex &node,
                   const IndexMap &vertex_index_map,
                   const sccalgorithms::DirectedGraph &graph,
                   vector<int> &r_index,
                   stack <int> &stack,
                   int &c,
                   int &index_component
)
{
    unsigned int num_sccs = 0;
    int index = boost::get(vertex_index_map, node);
    bool root = true;
    r_index[index] = index_component;
    index_component++;

    for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
    {
        int index_2 = boost::get(vertex_index_map,vd);

        if (r_index[index_2] == 0)
        {
            num_sccs += pearce2_visit(vd, vertex_index_map, graph, r_index, stack, c, index_component);
        }
        if (r_index[index_2] < r_index[index])
        {
            r_index[index] = r_index[index_2];
            root = false;
        }
    }

    if (root) {
        num_sccs++;
        index_component --;
        //cout<<"New component found: "<<node;
        while (!stack.empty() && r_index[index] <= r_index[stack.top()])
        {
            int index_2 = stack.top();
            stack.pop();
            r_index[index_2] = c;
            index_component --;
            //cout<<", "<<index_2;
        }
        r_index[index] = c;
        c--;
        //cout<<endl;
    }
    else {
        stack.push(index);
    }
    return num_sccs;
}


unsigned int sccalgorithms::pearce2_scc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{

    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);

    std::vector<int>  r_index(l);
    std::stack <int> stack;
    int index = 1;
    int c = l-1;

    for (int i = 0;i<l;i++)
    {
        r_index[i] = 0;
    }

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
            int int_value = boost::get(vertex_index_map,v);
            if (r_index[int_value] == 0)
            {
                num_sccs += pearce2_visit(v,vertex_index_map, graph, r_index, stack, c, index);
            }
        }

    return num_sccs;
}