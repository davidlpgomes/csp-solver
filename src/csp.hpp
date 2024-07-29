#ifndef CSP_HPP
#define CSP_HPP

#include <queue>
#include <set>
#include <vector>

#include "restriction.hpp"

namespace csp {

class Csp {
    public:
        Csp();

        virtual ~Csp();

        static Csp *fromInput();

        void removeValueFromVarDomain(unsigned xi, unsigned di);
        void resetVarDomain(unsigned xi);

        void print();

        unsigned numVars;
        unsigned numRestr;

        // Domains: vector of vector, which vector represents the sorted domain
        // of the variable at index i + 1.
        std::vector<std::vector<int>> domains;
        std::vector<unsigned> domainsValidSize;
        std::vector<std::vector<bool>> domainsIdxValid;

        std::vector<Restriction *> restrictions;

        // For each variable, stores the index of the restrictions that has the
        // var in the scope (pair: scope index at restriction, pointer to
        // restriction)
        std::vector<std::vector<std::pair<unsigned, Restriction *>>>
            variablesRestrictions;

        // Stores the tuples that were invalidaded by each variable
        std::vector<std::queue<unsigned *>> tuplesInvalidadedAddrs;
};

}; // namespace csp

#endif
