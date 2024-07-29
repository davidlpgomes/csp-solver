#ifndef RESTRICTION_HPP
#define RESTRICTION_HPP

#include <map>
#include <vector>

namespace csp {

class Restriction {
    public:
        Restriction();

        virtual ~Restriction() = default;

        static Restriction *fromInput();

        // Restriction "ID", starts at 1
        unsigned numRestr;

        // type: 0 -> invalid values, 1 -> valid values
        int type;

        unsigned scopeSize;
        unsigned tupleQty;

        std::map<unsigned, unsigned> varsIdxAtTuple;
        std::vector<unsigned> scope;

        // Uses vector due to possible scope increase
        std::vector<std::vector<int>> tuples;
        std::vector<unsigned> tuplesInvalidCount;
};

} // namespace csp

#endif
