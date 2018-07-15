//
// Created by andrea on 15/07/18.
//

#ifndef SCC_CONSOLE_FORMATTER_H
#define SCC_CONSOLE_FORMATTER_H

#include "scc_reports.h"
#include <chrono>

using namespace std;

class console_formatter : public report_formatter{
public:
    console_formatter(const std::vector<scc_record> & records,
                     const std::vector<std::string> & algorithms_names) : report_formatter(records, algorithms_names) {}

    inline void print_header(std::ostream & os) const {
        os << "| N | V | sccs | ";
        for (const auto & algorithm_name : algorithms_names)
            os << algorithm_name << " | ";
        os << endl;
    };

    inline void print_record(std::ostream & os, const scc_record & record) const {
        unsigned v, e, sccs;
        vector<tuple<unsigned, bool> > algorithm_results;

        tie(v, e, sccs, algorithm_results) = record;
        os << "| " << v << " | " << e << " | " << sccs << " | ";

        unsigned duration;
        bool isCorrect;
        for (const auto & p : algorithm_results) {
            tie(duration, isCorrect) = p;
            os << duration << (isCorrect ? "v" : "X") << " | ";
        }

        os << endl;
    };
};


#endif //SCC_CONSOLE_FORMATTER_H
