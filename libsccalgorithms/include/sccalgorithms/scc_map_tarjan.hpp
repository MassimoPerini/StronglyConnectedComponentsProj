//
// Created by andrea on 25/07/18.
//

#ifndef SCC_SCC_MAP_TARJAN_HPP
#define SCC_SCC_MAP_TARJAN_HPP

#include <boost/property_map/property_map.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/graph_traits.hpp>

#include <vector>
#include <stack>
#include <numeric>

namespace sccalgorithms {

    namespace tarjan_details {

        constexpr int NOT_ASSIGNED_NUMBER = 0;

        template <class Graph, class ComponentMap, class LowptProp, class LowvineProp, class NumberProp, class ContainsVertexProp>
        void
        tarjan_map_visit (
                const Graph & graph,
                typename boost::graph_traits<Graph>::vertex_descriptor v,
                ComponentMap comp,
                LowptProp lowpts,
                LowvineProp lowvines,
                NumberProp numbers,
                ContainsVertexProp is_on_stack,
                std::stack<typename boost::graph_traits<Graph>::vertex_descriptor> & visited,
                typename boost::property_traits<NumberProp>::value_type & i,
                typename boost::property_traits<ComponentMap>::value_type & num_sccs)
        {
            boost::put(numbers, v, ++i);
            boost::put(lowpts, v, i);
            boost::put(lowvines, v, i);
            boost::put(is_on_stack, v, true);
            visited.push(v);

            BGL_FORALL_ADJ_T(v, w, graph, Graph) {
                if (boost::get(numbers, w) == NOT_ASSIGNED_NUMBER) {
                    // (v, w) is a tree arc;
                    tarjan_map_visit(graph, w, comp, lowpts, lowvines, numbers, is_on_stack, visited, i, num_sccs);
                    boost::put(lowpts, v,
                               std::min(boost::get(lowpts, v),
                                        boost::get(lowpts, w)));
                    boost::put(lowvines, v,
                               std::min(boost::get(lowvines, v),
                                        boost::get(lowvines, w)));
                } else if (boost::get(lowpts, w) == boost::get(numbers, w) && // v is anchestor of w
                           boost::get(numbers, w) < boost::get(numbers, v) &&
                           boost::get(is_on_stack, w)) {
                    boost::put(lowpts, v,
                               std::min(
                                       boost::get(lowpts, v),
                                       boost::get(numbers, w)));
                } else if (boost::get(numbers, w) < boost::get(numbers, v) &&
                           boost::get(is_on_stack, w)) {
                    boost::put(lowvines, v,
                               std::min(
                                       boost::get(lowvines, v),
                                       boost::get(numbers, w)));
                }
            }

            if(boost::get(lowpts, v) == boost::get(numbers, v) &&
               boost::get(lowvines, v) == boost::get(numbers, v)) {
                // start new connected componet

                while (!visited.empty() && boost::get(numbers, visited.top()) >= boost::get(numbers, v)) {
                    boost::put(is_on_stack, visited.top(), false);
                    boost::put(comp, visited.top(), num_sccs);
                    visited.pop();
                }

                ++num_sccs;
            }
        }; // tarjan_map_visit

        template <class Graph, class ComponentMap, class LowptProp, class LowvineProp, class NumberProp, class ContainsVertexProp>
        inline typename boost::property_traits<ComponentMap>::value_type
        tarjan_map_helper (
                const Graph & graph,
                ComponentMap comp,
                LowptProp lowpts,
                LowvineProp lowvines,
                NumberProp numbers,
                ContainsVertexProp is_on_stack,
                std::stack<typename boost::graph_traits<Graph>::vertex_descriptor> & visited)
        {


            typename boost::property_traits<NumberProp>::value_type i = 0;
            typename boost::property_traits<ComponentMap>::value_type num_sccs = 0;

            BGL_FORALL_VERTICES_T(v, graph, Graph)
            {
                if (boost::get(numbers, v) == NOT_ASSIGNED_NUMBER)
                    tarjan_map_visit(graph, v,
                                     comp, lowpts, lowvines, numbers, is_on_stack, visited, i, num_sccs);
            }

            return num_sccs;
        }; // tarjan_map_helper

    }; // namespace tarjan_details


    template <class Graph, class ComponentMap>
    inline typename boost::property_traits<ComponentMap>::value_type
    tarjan_map_scc (
            const Graph & graph,
            ComponentMap comp)
    {
        using namespace tarjan_details;
        typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
        BOOST_CONCEPT_ASSERT(( boost::ReadWritePropertyMapConcept<ComponentMap, Vertex> ));
        typedef typename boost::graph_traits<Graph>::vertices_size_type size_type;


        typename boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map =
                boost::get(boost::vertex_index, graph);

        // LOWPT, LOWVINE exterior pointer properties
        size_type numVertices = boost::num_vertices(graph);
        std::vector<size_type> lowpt(numVertices);
        std::vector<size_type> lowvine(numVertices);
        std::vector<size_type> numbers(numVertices, NOT_ASSIGNED_NUMBER);
        std::vector<bool> is_on_stack(numVertices, false);
        std::stack<Vertex> stackVertexes;

        return tarjan_map_helper (
                graph,
                comp,
                boost::make_iterator_property_map(lowpt.begin(), vertex_index_map),
                boost::make_iterator_property_map(lowvine.begin(), vertex_index_map),
                boost::make_iterator_property_map(numbers.begin(), vertex_index_map),
                boost::make_iterator_property_map(is_on_stack.begin(), vertex_index_map),
                stackVertexes);
    }; // tarjan_map_scc

} // namespace sccalgorithms

#endif //SCC_SCC_MAP_TARJAN_HPP
