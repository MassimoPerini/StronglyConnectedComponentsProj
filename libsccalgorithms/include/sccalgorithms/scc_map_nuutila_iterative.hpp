//
// Created by Massimo Perini on 28/07/18.
//

#ifndef SCC_SCC_MAP_NUUTILA_ITERATIVE_HPP
#define SCC_SCC_MAP_NUUTILA_ITERATIVE_HPP

namespace sccalgorithms {

    namespace nuutila1_iterative_details {

        template <class Graph, class ComponentMap>
        inline int nuutila1_visit(unsigned int &traversal_counter,
                                   const typename boost::property_map<Graph, boost::vertex_index_t>::const_type &vertex_index_map,
                                   const Graph &graph,
                                   std::vector<unsigned int> &visitation_index,
                                   std::vector<int> &root,
                                   std::vector<bool> &index_components,
                                   std::stack<int> &stack1,
                                   std::stack<typename boost::graph_traits<Graph>::vertex_descriptor> &vS,
                                   std::stack <std::pair<typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator>> &iS,
                                   typename boost::property_traits<ComponentMap>::value_type &total_scc,
                                   ComponentMap components) {

            typename boost::graph_traits<Graph>::vertex_descriptor v = vS.top();
            std::pair <typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator > info;

            int vertex_index = boost::get(vertex_index_map, v);

            while (iS.top().first != iS.top().second) //I need to check a subgraph
            {
                typename boost::graph_traits<Graph>::vertex_descriptor new_node= (*(iS.top().first));
                int index = boost::get(vertex_index_map, new_node);

                if (visitation_index[index] >= 1) {

                    if (!index_components[index]) {
                        if (visitation_index[root[index]] < visitation_index[root[vertex_index]])
                            root[vertex_index] = root[index];
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

                    //info = iS.top();
                    typename boost::graph_traits<Graph>::vertex_descriptor new_node = (*(iS.top().first));
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

            if (root[vertex_index] == vertex_index) {
                count = 1;
                //cout<<"NEW COMPONENT FOUND: "<<vertex_index;
                index_components[vertex_index] = true;
                boost::put(components, vertex_index, total_scc);

                if (!stack1.empty()) {
                    int old_index = stack1.top();
                    while (visitation_index[old_index] > visitation_index[vertex_index]) {
                        //cout << " , " << old_index;
                        index_components[old_index] = true;
                        boost::put(components, old_index, total_scc);
                        stack1.pop();
                        if (stack1.empty()) {
                            break;
                        }
                        old_index = stack1.top();
                    }
                }
                total_scc++;
                //cout<<endl;
            } else {
                stack1.push(vertex_index);
            }

            return count;
        }
    };

    template <class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    nuutila1_map_iterative_scc(const Graph &graph, ComponentMap components)
    {
        using namespace nuutila1_iterative_details;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;

        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map = boost::get(boost::vertex_index, graph);

        typename boost::property_traits<ComponentMap>::value_type num_sccs = 0;
        typename boost::property_traits<ComponentMap>::value_type enumerate_scc = 0;

        size_type l = num_vertices(graph);

        std::vector<int>  roots(l);
        std::stack <int> stack1;
        std::vector<unsigned int> visitation_index(l);
        std::vector<bool> index_components(l);

        unsigned int traversal_counter = 1;

        std::stack <typename boost::graph_traits<Graph>::vertex_descriptor> vS;
        std::stack <std::pair <typename boost::graph_traits<Graph>::adjacency_iterator, typename boost::graph_traits<Graph>::adjacency_iterator > > iS;

        for (int i = 0;i<l;i++)
        {
            roots[i] = 0;
            index_components[i] = false;
            visitation_index[i] = 0;
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
                    num_sccs+=nuutila1_visit(traversal_counter, vertex_index_map, graph, visitation_index, roots, index_components, stack1, vS, iS, enumerate_scc, components);
                }
            }
        }

        return num_sccs;

    }

};

#endif //SCC_SCC_MAP_NUUTILA_ITERATIVE_HPP
