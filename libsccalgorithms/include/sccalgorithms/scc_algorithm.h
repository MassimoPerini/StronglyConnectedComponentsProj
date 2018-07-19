#ifndef SCC_SCC_ALGORITHM_H
#define SCC_SCC_ALGORITHM_H

#include <string>
#include <functional>
#include <sccalgorithms/scc_utils.h>

namespace sccalgorithms {
    class scc_algorithm {
    public:
        scc_algorithm(const char *name, const std::function<unsigned(const sccalgorithms::DirectedGraph &)> &func)
                : _name(name), _func(func) {}

        const std::string & getName() const {
            return _name;
        }

        unsigned operator()(const sccalgorithms::DirectedGraph & g) {
            return _func(g);
        }

    private:
        const std::string &_name;
        const std::function<unsigned(const sccalgorithms::DirectedGraph &)> &_func;
    };
}

#endif //SCC_SCC_ALGORITHM_H
