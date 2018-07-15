#include <iostream>
#include <map>
#include <string>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>

#include <sccalgorithms/scc_utils.h>
#include <sccalgorithms/scc_algorithms.h>
#include <scc_reports.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace sccalgorithms;
using namespace boost;

const map<string, string> PARAMS_HELP = {
        {"<MIN_V>",          "The minimum number of verteces to start generating the graphs."},
        {"<MAX_V>",          "The maximum number of verteces to stop in generating graphs"},
        {"<OFFSET_V>",       "The increment between one generated graph and next one."},
        {"<MIN_DENSITY>",    "The minimum density value used to populate graph edges"},
        {"<MAX_DENSITY>",    "The maximum density value used to populate graph edges"},
        {"<OFFSET_DENSITY>", "The increment of density between one generated graph and next one."}
};

/**
 * Prints program usage.
 */
void out_usage(ostream &, const string &, const map<string, string> &);

int main(int argc, char* argv[]) {

    if (argc < PARAMS_HELP.size()) {
        if (argc == 2 && (argv[1] == "--help" || argv[1] == "-h")) {
            out_usage(cout, argv[0], PARAMS_HELP);
            return EXIT_SUCCESS;
        }

        cerr << "Error: not enough params passed, " << PARAMS_HELP.size() << " params needed.\n";
        cerr << "Checks `" << argv[0] << " --help` for usage help." << endl;
        return EXIT_FAILURE;
    };

    unsigned minV;
    unsigned maxV;
    unsigned offsetV;
    float minDensity;
    float maxDensity;
    float offsetDensity;

    try {
        minV = lexical_cast<unsigned>(argv[1]);
        maxV = lexical_cast<unsigned>(argv[2]);
        offsetV = lexical_cast<unsigned>(argv[3]);
        minDensity = lexical_cast<float>(argv[4]);
        maxDensity = lexical_cast<float>(argv[5]);
        offsetDensity = lexical_cast<float>(argv[6]);
    } catch (const bad_lexical_cast & e) {
        cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    scc_reports report(minV, maxV, offsetV, minDensity, maxDensity, offsetDensity);
    report.run({
            [](const DirectedGraph & g) { return tarjan_scc(g);   },
            [](const DirectedGraph & g) { return nuutila1_ssc(g); },
            [](const DirectedGraph & g) { return nuutila2_ssc(g); },
            [](const DirectedGraph & g) { return pearce1_ssc(g);  },
            [](const DirectedGraph & g) { return pearce2_ssc(g);  },
    });

    return EXIT_SUCCESS;
}

/**
 * Prints program usage on sout.
 * @param sout the output stream to print in.
 * @param program_name the name of this executable.
 * @param params_help a map of parameters name and their meaning.
 */
void out_usage(ostream & sout, const string & program_name, const map<string, string> &params_help) {
    sout << "Usage:\n\n" << "\t" << program_name;

    for (const auto & pair : params_help)
        sout << " " << pair.first;

    sout << "\nwhere:\n";
    for (const auto & pair : params_help)
        cout << "\t- " << pair.first << " : " << pair.second << "\n";

    sout.flush();
}