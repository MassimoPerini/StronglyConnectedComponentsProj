#ifndef SCC_SCC_REPORTS_H
#define SCC_SCC_REPORTS_H

#include <vector>
#include <functional>
#include <tuple>
#include <sccalgorithms/scc_algorithms.h>

class scc_reports {
public:
    scc_reports(unsigned minV, unsigned maxV, unsigned offsetV, float minDensity,
                float maxDensity, float offsetDensity) : minV(minV), maxV(maxV), offsetV(offsetV),
                                                                minDensity(minDensity), maxDensity(maxDensity),
                                                                offsetDensity(offsetDensity) {}

    std::vector<std::tuple<unsigned, unsigned, unsigned, std::vector<std::tuple<unsigned, bool> > > >
            run(const std::vector<std::function<unsigned(const sccalgorithms::DirectedGraph &)> > &);

private:
    unsigned minV;
    unsigned maxV;
    unsigned offsetV;
    float minDensity;
    float maxDensity;
    float offsetDensity;
};


#endif //SCC_SCC_REPORTS_H
