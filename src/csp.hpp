#ifndef CSP_HPP
#define CSP_HPP

#include <set>
#include <vector>

#include "restriction.hpp"


namespace csp {

class Csp {
    public:
        Csp();

        virtual ~Csp();

        static Csp* fromInput();

        void print();
    
        unsigned numVars;
        unsigned numRestr;

        // Domains: vector of sets, which set represents the sorted domain of
        // the variable at index i + 1.
        std::vector<std::set<int>> domains;

        std::vector<Restriction*> restrictions;

};


}; // namespace csp

#endif
