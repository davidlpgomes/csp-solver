#ifndef RESTRICTION_HPP
#define RESTRICTION_HPP

#include <vector>


namespace csp {

class Restriction {
    public:
        Restriction();

        virtual ~Restriction() = default;

        static Restriction* fromInput();

        // type: 0 -> invalid values, 1 -> valid values
        int type;

        unsigned scopeSize;
        unsigned tupleQty;

        std::vector<unsigned> scope;

        // Uses vector due to possible scope increase
        std::vector<std::vector<int>> tuples;
};

} // namespace csp

#endif
