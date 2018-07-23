#include "scc_reports.h"
#include <iostream>
#include <numeric>
#include <chrono>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>


typedef boost::erdos_renyi_iterator<boost::minstd_rand, sccalgorithms::DirectedGraph> ERGen;

std::vector<scc_record>
        scc_reports::run(std::vector<sccalgorithms::scc_algorithm> & algorithms) {

    // save records here
    std::vector<scc_record> results;

    boost::minstd_rand gen;

    for (unsigned i=std::max(2u, minV); i<=maxV; i+=1) { // for each number of verteces
        for (float j=minDensity; j<=maxDensity; j+=offsetDensity) { // for each density value
            sccalgorithms::DirectedGraph randomGraph(ERGen(gen, i, j), ERGen(), i); // creates a random graph

            // computes the number of sccs from boost tarjan algorithm
            std::vector<int> component(boost::num_vertices(randomGraph));
            unsigned boost_num_sccs = boost::strong_components(randomGraph,
                    boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, randomGraph)));

            std::vector<std::tuple<unsigned, bool> > algorithms_result;
            for (auto & algorithm : algorithms) { // for each algorithm that computes scc given
                std::chrono::high_resolution_clock::time_point t1, t2;
                t1 = std::chrono::high_resolution_clock::now();

                unsigned algorithm_num_sccs = algorithm(randomGraph);

                t2 = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

                algorithms_result.push_back(std::make_tuple(duration, algorithm_num_sccs == boost_num_sccs));
            }

            results.push_back(std::make_tuple(i, boost::num_edges(randomGraph), boost_num_sccs, algorithms_result));
        }
    }

    return results;
}

std::ostream &operator<<(std::ostream & os, const report_formatter & rf) {
    rf.print_header(os);
    for (const auto & record : rf.records)
        rf.print_record(os, record);

    return os;
}
