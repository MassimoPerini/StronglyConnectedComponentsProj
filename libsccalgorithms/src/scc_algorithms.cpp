#include "sccalgorithms/scc_algorithms.h"
#include "sccalgorithms/scc_map_tarjan.hpp"
#include "sccalgorithms/scc_map_nuutila.hpp"
#include "sccalgorithms/scc_map_pearce.hpp"
#include <boost/graph/strong_components.hpp>

#define SCC_PICK(_f) static_cast<unsigned(*)(const sccalgorithms::DirectedGraph&)>(_f)

const std::vector<sccalgorithms::scc_algorithm> sccalgorithms::availableAlgorithms() {
    return {
            sccalgorithms::scc_algorithm("boost", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return boost::strong_components(g,
                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("tarjan_map", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return sccalgorithms::tarjan_map_scc(g,
                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("tarjan",   SCC_PICK(tarjan_scc)),
            sccalgorithms::scc_algorithm("nuutila1", SCC_PICK(nuutila1_scc)),
            sccalgorithms::scc_algorithm("nuutila1_map", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return sccalgorithms::nuutila1_map_scc(g,
                        boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("nuutila1_iterative", SCC_PICK(nuutila1_iterative_ssc)),
            sccalgorithms::scc_algorithm("nuutila2", SCC_PICK(nuutila2_scc)),
            sccalgorithms::scc_algorithm("nuutila1_map", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return sccalgorithms::nuutila1_map_scc(g,
                                                       boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("nuutila2_iterative", SCC_PICK(nuutila2_iterative_ssc)),
            sccalgorithms::scc_algorithm("pearce1",  SCC_PICK(pearce1_scc)),
            sccalgorithms::scc_algorithm("pearce1_map", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return sccalgorithms::pearce1_map_scc(g,
                                                      boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("pearce2",  SCC_PICK(pearce2_scc)),
            sccalgorithms::scc_algorithm("pearce2_map", [](const sccalgorithms::DirectedGraph & g) {
                std::vector<int> component(num_vertices(g));
                return sccalgorithms::pearce2_map_scc(g,
                                                      boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, g)));
            }),
            sccalgorithms::scc_algorithm("pearce2_iterative",  SCC_PICK(pearce2_iterative_ssc))
    };
};  