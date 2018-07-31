#include <iostream>
#include <map>
#include <string>
#include "csv_formatter.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <stdexcept>
#include <iterator>

#include <sccalgorithms/scc_algorithms.h>
#include <boost/lexical_cast.hpp>
#include <boost/property_map/property_map.hpp>

#include "scc_reports.h"
#include "console_formatter.h"


using namespace std;
using namespace sccalgorithms;

#define HELP_IF_FAIL(x) \
    try {\
        x;\
    } catch (const std::exception & e) {\
        std::cerr << "Error: " << e.what() << endl;\
        out_usage(cerr, argv[0], PARAMS_HELP, PARAMS_MEM_HELP);\
        return EXIT_FAILURE;\
    }

typedef std::tuple<unsigned, unsigned, unsigned, float, float, float> gen_graph_params;

template <class Iter>
gen_graph_params
readGenGraphParams(Iter idx, Iter last) {
    typename iterator_traits<Iter>::difference_type size = distance(idx, last);
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

const map<string, string> PARAMS_MEM_HELP = {
        {"<ALGORITHM_NAME>",  "The name of choosen algorithm to run memory analysis on (see above available ones)."},
        {"<V>",               "The number of verteces to generating the graph with."},
        {"<DENSITY_EDGE>",    "The density value used to populate graph edges."},
        {"[<OUTPUT_FILENAME>]", "The file name where to append the result (optional)."},
};

constexpr char OUTPUT_FILENAME[] = "report.csv";

/**
 * Prints program usage.
 */
void out_usage(ostream &, const string &, const map<string, string> &, const map<string, string> &);

/*
 * ./SCCApp --algorithm boost tarjan nuutila1 --gengraph 1 100 2 0.01 0.3 0.02
 * ./SCCApp -a boost tarjan nuutila1 -g 1 100 2 0.01 0.3 0.02
 * ./SCCApp --gengraph 1 100 2 0.01 0.3 0.02
 * ./SCCApp -g 1 100 2 0.01 0.3 0.02
 * ./SCCApp 1 100 2 0.01 0.3 0.02
 */
int timeAnalysis(int, char*[]);

/*
 * ./SCCApp --mem-analysis tarjan 100 0.1 outputFile.csv
 * ./SCCApp --mem-analysis <ALGORITHM_NAME> <V> <DENSITY_EDGE> <OUTPUT_FILENAME>
 */
int memoryAnalysis(int, char*[]);

int main(int argc, char* argv[]) {
    if (argc == 2 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)) {
        out_usage(cout, argv[0], PARAMS_HELP, PARAMS_MEM_HELP);
        return EXIT_SUCCESS;
    }

    if (argc > 1 && strcmp("--mem-analysis", argv[1]) == 0)
        return memoryAnalysis(argc, argv);
    else
        return timeAnalysis(argc, argv);
}

int timeAnalysis(int argc, char* argv[]) {
    auto sccAlgorithms = availableAlgorithms<DirectedGraph>(); // TODO availableAlgorithms as constant given ?

    vector<scc_algorithm<DirectedGraph> > chosenAlgorithms;
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

        HELP_IF_FAIL( genGraphTuple = readGenGraphParams(genGraphParams+1, end(_vargv)) )
    } else {
        HELP_IF_FAIL( genGraphTuple = readGenGraphParams(begin(_vargv), end(_vargv)) )
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

    scc_reports<DirectedGraph> report(minV, maxV, offsetV, minDensity, maxDensity, offsetDensity);
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

int memoryAnalysis(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Error, not enough parameters given." << endl;
        out_usage(cerr, argv[0], PARAMS_HELP, PARAMS_HELP);
        return EXIT_FAILURE;
    }

    auto sccAlgorithms = availableAlgorithms<DirectedGraph>();
    auto algorithmIter = find_if(begin(sccAlgorithms), end(sccAlgorithms), [&argv](const auto &alg) {return strcmp(alg.getName().c_str(), argv[2])==0;});
    if (algorithmIter == end(sccAlgorithms)) {
        cerr << "Error, cannot find the chosen algorithms `" << argv[2] << "`." << endl;
        out_usage(cerr, argv[0], PARAMS_HELP, PARAMS_MEM_HELP);
        return EXIT_FAILURE;
    }

    scc_algorithm<DirectedGraph> chosenAlgorithm = *algorithmIter;
    unsigned numV = boost::lexical_cast<unsigned>(argv[3]);
    float densityE = boost::lexical_cast<float>(argv[4]);
    string outputFileName = (argc == 6) ? argv[5] : "mem_report_" + chosenAlgorithm.getName() + ".csv";

    scc_record_memory recordResult = scc_reports_memory<DirectedGraph>(numV, densityE).run(chosenAlgorithm);
    unsigned numberOfV, numberOfE, sccs;
    size_t tare, algorithmPeak, difference;
    string algorithmName;
    std::tie(numberOfV, numberOfE, sccs, tare, algorithmPeak, difference, algorithmName) = recordResult;

    fstream csvFs(outputFileName.c_str(), std::fstream::app);
    cout << "| V | E | sccs | tare | algorithmPeak | delta | algorithmName |\n";
    auto out_joint = [&](string sep, ostream & out) {
        out << numberOfV << sep << numberOfE << sep << sccs
            << sep << tare << sep << algorithmPeak << sep << difference
            << sep << algorithmName << endl;

    };

    cout << "| ";
    out_joint(" | ", cout);
    out_joint(",", csvFs);
    cout << endl << "Recort appended to file " << outputFileName << endl;

    csvFs.close();
    return EXIT_SUCCESS;
}

/**
 * Prints program usage on sout.
 * @param sout the output stream to print in.
 * @param program_name the name of this executable.
 * @param params_help a map of parameters name and their meaning.
 */
void out_usage(ostream & sout, const string & program_name, const map<string, string> &params_help, const map<string, string> &params_mem_help) {
    sout << "1) Time Analysis: \n";
    sout << "Usage:\n\n" << "\t" << program_name;

    sout << " [--algorithm {ALGORITHM_NAME} --gengraph] ";
    for (const auto & pair : params_help)
        sout << " " << pair.first;

    sout << "\nwhere:\n";
    sout << "\t - <ALGORITHM-NAME> : The name of a scc algorithm we implemented.\n"
         << "\t     If the list provided by param --algorithm is empty, all those available will be used.\n"
         << "\t     Choose among: ";
    for (const auto & alg : availableAlgorithms<DirectedGraph>())
        sout << alg << " ";
    sout << "\n";

    for (const auto & pair : params_mem_help)
        sout << "\t- " << pair.first << " : " << pair.second << "\n";

    sout << "\n2) Memory Analisys: \n";
    sout << "Usage:\n\n" << "\t"
        << program_name << " --mem-analysis ";

    for (const auto & pair : params_mem_help)
        sout << " " << pair.first;

    sout << "\nwhere:\n";
    for (const auto & pair : params_mem_help)
        sout << "\t- " << pair.first << " : " << pair.second << "\n";

    sout.flush();
}

