#include "sccalgorithms/scc_algorithms.h"
#include <boost/graph/iteration_macros.hpp>
#include <boost/property_map/property_map.hpp>


#include <iostream>
#include <stack>

//using vertex_descriptor = boost::graph_traits<sccalgorithms::DirectedGraph>::vertex_descriptor;
using namespace boost;
using namespace std;
//typedef boost::property_map<vertex_descriptor, boost::vertex_index_t>::type VertexIndexMap;
//typedef property_map<vertex_descriptor, vertex_index_t> IndexMap;

using vertex_descriptor = boost::graph_traits<sccalgorithms::DirectedGraph>::vertex_descriptor;

unsigned int nuutila1_visit(const vertex_descriptor node,
                    int &depth,
                   const IndexMap &vertex_index_map,
                   const sccalgorithms::DirectedGraph &graph,
                   vector<unsigned int> &visitation_index,
                   vector<int> &root,
                   vector<bool> &index_components,
                   stack <int> &stack)
{
    unsigned int num_sccs = 0;
    int index = boost::get(vertex_index_map, node);
    root[index] = index;
    index_components[index] = false;
    visitation_index [index] = depth;

    for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
    {
        int index_2 = boost::get(vertex_index_map,vd);
        if (visitation_index[index_2] < 1)//not visited -> explore
        {
            depth++;
            num_sccs += nuutila1_visit(vd, depth, vertex_index_map, graph, visitation_index, root, index_components, stack);
        }

        if (!index_components[index_2])
        {
            if (visitation_index[root[index_2]] < visitation_index[root[index]])
                root[index] = root[index_2];
        }
    }

    if (root[index] == index)
    {
        num_sccs++;
        //cout<<"NEW COMPONENT FOUND: "<<index;
        index_components[index] = true;
        if (!stack.empty()) {
            int old_index = stack.top();
            while (visitation_index[old_index] > visitation_index [index]) {
                //cout << " , " << old_index;
                index_components[old_index] = true;
                stack.pop();
                if (stack.empty()) {
                    break;
                }
                old_index = stack.top();
            }
        }
        //cout<<endl;
    }
    else {
        stack.push(index);  //oppure pushare il nodo direttamente e poi prendere al pop il suo index
    }
    return num_sccs;
}


unsigned int sccalgorithms::nuutila1_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{

    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);
    std::vector<int> roots(l);
    std::vector<unsigned int> visitation_indexes(l);
    std::vector<bool> index_components(l);
    std::stack <int> stack;
    int depth = 1;

    for (int i = 0;i<l;i++)
    {
        roots[i] = 0;
        index_components[i] = false;
        visitation_indexes[i] = 0;
    }

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
            int int_value = boost::get(vertex_index_map,v);
            if (visitation_indexes[int_value] == 0)
            {
                num_sccs += nuutila1_visit(v, depth,vertex_index_map, graph, visitation_indexes,roots, index_components, stack);
            }
        }

    return num_sccs;
}

unsigned int sccalgorithms::nuutila1_ssc(const sccalgorithms::DirectedGraph &graph)
{
    return sccalgorithms::nuutila1_ssc(graph, boost::get(boost::vertex_index, graph));
}




unsigned int nuutila2_visit(const vertex_descriptor node,
                    int &depth,
                    const IndexMap &vertex_index_map,
                    const sccalgorithms::DirectedGraph &graph,
                    vector<unsigned int> &visitation_index,
                    vector<int> &root,
                    vector<bool> &index_components,
                    stack <int> &stack,
                    vector<bool> &is_on_stack)
{
    unsigned int num_sccs = 0;
    int index = boost::get(vertex_index_map, node);
    root[index] = index;
    index_components[index] = false;
    visitation_index [index] = depth;

    for (auto vd : boost::make_iterator_range(adjacent_vertices(node, graph)))  //cosa cambia tra inv e non inv?
    {
        int index_2 = boost::get(vertex_index_map,vd);
        if (visitation_index[index_2] < 1)//not visited -> explore
        {
            depth++;
            num_sccs += nuutila2_visit(vd, depth, vertex_index_map, graph, visitation_index, root, index_components, stack, is_on_stack);
        }

        if (!index_components[root[index_2]])
        {
            if (visitation_index[root[index_2]] < visitation_index[root[index]])
                root[index] = root[index_2];
        }
    }

    if (root[index] == index) {
        num_sccs++;
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
    }
    else if (!is_on_stack[root[index]]) {
        stack.push(root[index]);
        is_on_stack[root[index]] = true;
    }
    return num_sccs;
}


unsigned int sccalgorithms::nuutila2_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
{

    unsigned int num_sccs = 0;
    unsigned long l= num_vertices(graph);
    std::vector<int> roots(l);
    std::vector<unsigned int> visitation_indexes(l);
    std::vector<bool> index_components(l);
    std::stack <int> stack;
    std::vector<bool> is_on_stack(l);
    int depth = 1;

    for (int i = 0;i<l;i++)
    {
        roots[i] = 0;
        index_components[i] = false;
        visitation_indexes[i] = 0;
        is_on_stack[i] = false;
    }

    BGL_FORALL_VERTICES(v, graph, DirectedGraph)
        {
            //std::cout<<"TRUE VALUE: "<<boost::get(vertex_index_map,v) <<"\n";
            int int_value = boost::get(vertex_index_map,v);
            if (visitation_indexes[int_value] == 0)
            {
                 num_sccs += nuutila2_visit(v, depth,vertex_index_map, graph, visitation_indexes,roots, index_components, stack, is_on_stack);
            }
        }

    return num_sccs;
}

unsigned int sccalgorithms::nuutila2_ssc(const sccalgorithms::DirectedGraph &graph)
{
    return sccalgorithms::nuutila2_ssc(graph, boost::get(boost::vertex_index, graph));
}




unsigned int pearce1_visit(const vertex_descriptor node,
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


unsigned int sccalgorithms::pearce1_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
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

unsigned int sccalgorithms::pearce1_ssc(const sccalgorithms::DirectedGraph &graph)
{
    return sccalgorithms::pearce1_ssc(graph, boost::get(boost::vertex_index, graph));
}



unsigned int pearce2_visit(const vertex_descriptor node,
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


unsigned int sccalgorithms::pearce2_ssc(const sccalgorithms::DirectedGraph &graph, IndexMap vertex_index_map)
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

unsigned int sccalgorithms::pearce2_ssc(const sccalgorithms::DirectedGraph &graph)
{
    return sccalgorithms::pearce2_ssc(graph, boost::get(boost::vertex_index, graph));
}
