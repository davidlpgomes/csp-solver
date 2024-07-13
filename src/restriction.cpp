#include "restriction.hpp"

#include <iostream>

using namespace csp;

Restriction::Restriction() {}

Restriction *Restriction::fromInput() {
    Restriction *r{new Restriction};

    char type;
    std::cin >> type;
    r->type = type == 'V' ? 1 : 0;

    std::cin >> r->scopeSize;

    unsigned v;
    for (unsigned i = 0; i < r->scopeSize; i++) {
        std::cin >> v;
        r->scope.push_back(v);
    }

    std::cin >> r->tupleQty;

    for (unsigned i = 0; i < r->tupleQty; i++) {
        std::vector<int> tuple;

        for (unsigned j = 0; j < r->scopeSize; j++) {
            std::cin >> v;
            tuple.push_back(v);
        }

        r->tuples.push_back(tuple);
    }

    return r;
}
