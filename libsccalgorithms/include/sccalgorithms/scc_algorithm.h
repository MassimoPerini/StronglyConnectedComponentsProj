#ifndef SCC_SCC_ALGORITHM_H
#define SCC_SCC_ALGORITHM_H

#include <string>
#include <functional>
#include <sccalgorithms/scc_utils.h>

namespace sccalgorithms {
    class scc_algorithm {
    public:
        scc_algorithm(const char *name, const std::function<unsigned(const sccalgorithms::DirectedGraph &)> func)
                : _name(name), _func(func) {}

        const std::string getName() const {
            return _name;
        }

        unsigned int operator()(const sccalgorithms::DirectedGraph & g) {
            return _func(g);
        }

    private:
        std::string _name;
        std::function<unsigned(const sccalgorithms::DirectedGraph &)> _func;
    };

    inline std::ostream& operator<<(std::ostream & out, const scc_algorithm & alg) {
        return out << alg.getName();
    }
}

#endif //SCC_SCC_ALGORITHM_H
