#ifndef SCC_SCC_ALGORITHM_H
#define SCC_SCC_ALGORITHM_H

#include <string>
#include <functional>
#include <sccalgorithms/scc_utils.h>
#include <boost/property_map/property_map.hpp>

namespace sccalgorithms {

    template <class Graph, class ComponentMap = typename boost::iterator_property_map<typename std::vector<int>::iterator, typename boost::property_map<Graph, boost::vertex_index_t>::const_type> >
    class scc_algorithm {
    public:
        scc_algorithm(const char *name,
                      const std::function<typename boost::property_traits<ComponentMap>::value_type(const Graph &, ComponentMap)> func)
                : _name(name), _func(func) {}

        const std::string getName() const {
            return _name;
        }

        unsigned int operator()(const Graph & g, ComponentMap components) {
            return _func(g, components);
        }

    private:
        std::string _name;
        std::function<typename boost::property_traits<ComponentMap>::value_type(const Graph &, ComponentMap)> _func;
    };

    template <class Graph, class ComponentMap>
    inline std::ostream& operator<<(std::ostream & out, const scc_algorithm<Graph, ComponentMap> & alg) {
        return out << alg.getName();
    }
}

#endif //SCC_SCC_ALGORITHM_H
