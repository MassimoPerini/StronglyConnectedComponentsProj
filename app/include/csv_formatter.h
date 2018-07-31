#ifndef SCC_CSV_FORMATTER_H
#define SCC_CSV_FORMATTER_H

#include "scc_reports.h"

using namespace std;

class csv_formatter : public report_formatter{
public:
    csv_formatter(const std::vector<scc_record> & records,
                     const std::vector<std::string> & algorithms_names) : report_formatter(records, algorithms_names) {}

    inline void print_header(std::ostream & os) const {
        os << "V" << SEP << "E" << SEP << "sccs";
        for (const auto & algorithm_name : algorithms_names)
            os << SEP << algorithm_name << SEP << algorithm_name + "Corrected";
        os << endl;
    };

    inline void print_record(std::ostream & os, const scc_record & record) const {
        unsigned v, e, sccs;
        vector<tuple<unsigned, bool> > algorithm_results;

        tie(v, e, sccs, algorithm_results) = record;
        os << v << SEP << e << SEP << sccs;

        unsigned duration;
        bool isCorrect;
        for (const auto & p : algorithm_results) {
            tie(duration, isCorrect) = p;
            os << SEP << duration << SEP << (isCorrect ? "v" : "X");
        }

        os << endl;
    };
private:
    static constexpr char SEP = ',';
};


#endif //SCC_CSV_FORMATTER_H
