#ifndef SCC_SCC_REPORTS_H
#define SCC_SCC_REPORTS_H

#include <vector>
#include <functional>
#include <tuple>
#include <sccalgorithms/scc_algorithms.h>

typedef std::tuple<unsigned, unsigned, unsigned, std::vector<std::tuple<unsigned, bool> > > scc_record;

/**
 * Generates a report running algorithms provided on random graphs
 */
class scc_reports {
public:
    scc_reports(unsigned minV, unsigned maxV, unsigned offsetV, float minDensity,
                float maxDensity, float offsetDensity) : minV(minV), maxV(maxV), offsetV(offsetV),
                                                                minDensity(minDensity), maxDensity(maxDensity),
                                                                offsetDensity(offsetDensity) {}

    std::vector<scc_record>
            run(const std::vector<std::function<unsigned(const sccalgorithms::DirectedGraph &)> > &);

private:
    unsigned minV;
    unsigned maxV;
    unsigned offsetV;
    float minDensity;
    float maxDensity;
    float offsetDensity;
};


class report_formatter {
public:
    report_formatter(const std::vector<scc_record> & records,
                     const std::vector<std::string> & algorithms_names) : records(records), algorithms_names(algorithms_names) {}

    virtual void print_header(std::ostream &) const = 0;
    virtual void print_record(std::ostream &, const scc_record &) const = 0;

    friend std::ostream& operator<<(std::ostream &, const report_formatter &);

private:
    const std::vector<scc_record> records;

protected:
    const std::vector<std::string> algorithms_names;
};

std::ostream& operator<<(std::ostream&, const report_formatter&);

#endif //SCC_SCC_REPORTS_H
