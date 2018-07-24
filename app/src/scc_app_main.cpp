#include <iostream>
#include <map>
#include <string>
#include <sccalgorithms/scc_utils.h>
#include <sccalgorithms/scc_algorithms.h>
#include <scc_reports.h>
#include <boost/lexical_cast.hpp>
#include <console_formatter.h>
#include <csv_formatter.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <stdexcept>
#include <iterator>


#include <iostream>
#include <fstream>


using namespace std;
using namespace sccalgorithms;

#define HELP_IF_FAIL(x) \
    try {\
        x;\
    } catch (const std::exception & e) {\
        std::cerr << "Error: " << e.what() << endl;\
        out_usage(cerr, argv[0], PARAMS_HELP);\
        return EXIT_FAILURE;\
    }

typedef std::tuple<unsigned, unsigned, unsigned, float, float, float> gen_graph_params;

gen_graph_params readGenGraphParams(std::vector<std::string> & vec, std::vector<std::string>::iterator idx) {
    auto size = std::count_if(idx, std::end(vec), [](const auto & s){return true;});
    if (size < 6) throw std::out_of_range ("Error: at least 6 params needed, given " + boost::lexical_cast<std::string>(6));
    return std::make_tuple(
            boost::lexical_cast<unsigned>(*idx),
            boost::lexical_cast<unsigned>(*(idx+1)),
            boost::lexical_cast<unsigned>(*(idx+2)),
            boost::lexical_cast<float>(*(idx+3)),
            boost::lexical_cast<float>(*(idx+4)),
            boost::lexical_cast<float>(*(idx+5))
    );
}

const map<string, string> PARAMS_HELP = {
        {"<MIN_V>",          "The minimum number of verteces to start generating the graphs."},
        {"<MAX_V>",          "The maximum number of verteces to stop in generating graphs"},
        {"<OFFSET_V>",       "The increment between one generated graph and next one."},
        {"<MIN_DENSITY>",    "The minimum density value used to populate graph edges"},
        {"<MAX_DENSITY>",    "The maximum density value used to populate graph edges"},
        {"<OFFSET_DENSITY>", "The increment of density between one generated graph and next one."}
};

constexpr char OUTPUT_FILENAME[] = "report.csv";

/**
 * Prints program usage.
 */
void out_usage(ostream &, const string &, const map<string, string> &);

/*
 * ./SCCApp --algorithm boost tarjan nuutila1 --gengraph 1 100 2 0.01 0.3 0.02
 * ./SCCApp -a boost tarjan nuutila1 -g 1 100 2 0.01 0.3 0.02
 * ./SCCApp --gengraph 1 100 2 0.01 0.3 0.02
 * ./SCCApp -g 1 100 2 0.01 0.3 0.02
 * ./SCCApp 1 100 2 0.01 0.3 0.02
 */
int main(int argc, char* argv[]) {
    if (argc == 2 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)) {
        out_usage(cout, argv[0], PARAMS_HELP);
        return EXIT_SUCCESS;
    }

    auto sccAlgorithms = availableAlgorithms(); // TODO availableAlgorithms as constant given ?

    vector<scc_algorithm> chosenAlgorithms;
    gen_graph_params genGraphTuple;
    vector<string> _vargv(argv+1, argv+argc);

    auto genGraphParams = find(begin(_vargv), end(_vargv), "--gengraph");
    if (genGraphParams != end(_vargv)) {
        auto algorithmParams = find(begin(_vargv), end(_vargv), "--algorithm");
        if (algorithmParams != end(_vargv)) {

            for (auto idxAlg = algorithmParams + 1; idxAlg != end(_vargv) && idxAlg != genGraphParams; ++idxAlg) {
                auto algorithmIdx = find_if(begin(sccAlgorithms), end(sccAlgorithms), [idxAlg](const auto &alg) {return alg.getName()==*idxAlg;});
                if (algorithmIdx != end(sccAlgorithms))
                    chosenAlgorithms.push_back(*algorithmIdx);
            }
        }

        if (chosenAlgorithms.size() == 0)
            chosenAlgorithms = sccAlgorithms;

        HELP_IF_FAIL(genGraphTuple = readGenGraphParams(_vargv, genGraphParams+1))
    } else {
        HELP_IF_FAIL(genGraphTuple = readGenGraphParams(_vargv, begin(_vargv)))
        chosenAlgorithms = sccAlgorithms;
    }

    unsigned minV;
    unsigned maxV;
    unsigned offsetV;
    float minDensity;
    float maxDensity;
    float offsetDensity;
    std::tie(minV, maxV, offsetV, minDensity, maxDensity, offsetDensity) = genGraphTuple;

    vector<string> algorithms_names(chosenAlgorithms.size());
    transform(chosenAlgorithms.begin(), chosenAlgorithms.end(), algorithms_names.begin(), [](const auto & algo){return algo.getName();});

    scc_reports report(minV, maxV, offsetV, minDensity, maxDensity, offsetDensity);
    const auto records = report.run(chosenAlgorithms);

    console_formatter consoleReport(records, algorithms_names);
    cout << consoleReport;

    csv_formatter fileReport(records, algorithms_names);
    ofstream outputStream(OUTPUT_FILENAME, std::ofstream::out);
    outputStream << fileReport;
    outputStream.close();
    cout << "\nWritten report to file: " << OUTPUT_FILENAME << endl;

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

    sout << " [--algorithm {ALGORITHM_NAME} --gengraph] ";
    for (const auto & pair : params_help)
        sout << " " << pair.first;

    sout << "\nwhere:\n";
    sout << "\t - <ALGORITHM-NAME> : The name of a scc algorithm we implemented.\n"
         << "\t     If the list provided by param --algorithm is empty, all those available will be used.\n"
         << "\t     Choose among: ";
    for (const auto & alg : availableAlgorithms())
        sout << alg << " ";
    sout << "\n";

    for (const auto & pair : params_help)
        sout << "\t- " << pair.first << " : " << pair.second << "\n";

    sout.flush();
}

