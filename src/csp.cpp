#include "csp.hpp"
#include "debug.hpp"

#include <iostream>

using namespace csp;

Csp::Csp() {};

Csp::~Csp() {
    std::vector<Restriction *>::iterator it{this->restrictions.begin()};

    for (; it != this->restrictions.end(); ++it)
        if (*it != nullptr)
            delete *it;
}

void cleanDomain(Csp *csp) {
    /* If there is a restriction with type VALID with some variable in its
     * scope, the values that are not present in some tuple are removed from the
     * variable's domain */
    std::vector<Restriction *>::iterator it{csp->restrictions.begin()};
    std::vector<std::vector<int>>::iterator itTuples;
    std::vector<int>::iterator itTuple;
    Restriction *r;

    std::vector<std::set<int>> newDomains(csp->numVars);

    for (; it != csp->restrictions.end(); ++it) {
        r = *it;

        if (r->type == 0)
            continue;

        itTuples = r->tuples.begin();
        for (; itTuples != r->tuples.end(); ++itTuples) {
            int tupleIdx = 0;
            itTuple = itTuples->begin();

            for (; itTuple != itTuples->end(); ++itTuple) {
                newDomains[r->scope[tupleIdx] - 1].insert(*itTuple);
                tupleIdx++;
            }
        }
    }

    for (unsigned i = 0; i < csp->numVars; i++) {
        if (newDomains[i].size() == 0)
            continue;

        csp->domains[i] = newDomains[i];
    }

    return;
}

Csp *Csp::fromInput() {
    Csp *csp{new Csp};

    std::cin >> csp->numVars;

    for (unsigned i = 0; i < csp->numVars; i++) {
        std::vector<std::pair<unsigned, Restriction *>> res;
        csp->variablesRestrictions.push_back(res);
    }

    unsigned domainSize, v;

    for (unsigned i = 0; i < csp->numVars; i++) {
        std::cin >> domainSize;
        std::set<int> domain;

        for (unsigned j = 0; j < domainSize; j++) {
            std::cin >> v;
            domain.emplace(v);
        }

        csp->domains.push_back(domain);
    }

    std::cin >> csp->numRestr;

    Restriction *r;
    unsigned restId = 1;

    for (unsigned i = 0; i < csp->numRestr; i++) {
        r = Restriction::fromInput();

        // If scope size is 1, change the variable's domain
        // instead of adding another restriction
        if (r->scopeSize == 1 && r->type == 0) {
            std::set<int> &d = csp->domains[r->scope[0] - 1];
            std::vector<std::vector<int>>::iterator itScope;

            if (r->type == 0) {
                // Invalid values, remove them from domain
                for (itScope = r->tuples.begin(); itScope != r->tuples.end();
                     ++itScope)
                    d.erase((*itScope)[0]);
            } else {
                // Valid values, remove from the domain the values that
                // are not allowed
                std::set<int>::iterator itDomain;
                std::set<int> scopeSingleValues;

                for (itScope = r->tuples.begin(); itScope != r->tuples.end();
                     ++itScope)
                    scopeSingleValues.insert((*itScope)[0]);

                std::set<int>::iterator itSingle;

                for (itDomain = d.begin(); itDomain != d.end();) {
                    itSingle = scopeSingleValues.find(*itDomain);

                    if (itSingle != scopeSingleValues.end()) {
                        ++itDomain;
                        continue;
                    }

                    itDomain = d.erase(itDomain);
                }
            }

            delete r;
            continue;
        }

        r->numRestr = restId;
        restId++;

        for (unsigned i = 0; i < r->scopeSize; i++)
            csp->variablesRestrictions[r->scope[i] - 1].push_back(
                std::make_pair(i, r));

        csp->restrictions.push_back(r);
    }

    csp->numRestr = csp->restrictions.size();

    cleanDomain(csp);

    return csp;
}

void Csp::print() {
    DPRINT("Número de variáveis: %u\n", this->numVars);

    for (unsigned i = 0; i < this->numVars; i++) {
        std::set<int>::iterator it{this->domains[i].begin()};

        DPRINT("\tDomínio x%u: ", i + 1);

        for (; it != this->domains[i].end(); ++it)
            DPRINT("%d ", *it);

        DPRINT("\n");
    }

    DPRINT("Número de restrições: %u\n", this->numRestr);

    Restriction *r;
    for (unsigned i = 0; i < this->numRestr; i++) {
        r = this->restrictions[i];
        DPRINT("\tRestrição %u:\n", i + 1);

        DPRINT("\t\tTipo: valores ");
        if (!r->type) {
            DPRINT("inválidos");
        } else {
            DPRINT("válidos");
        }
        DPRINT("\n");

        DPRINT("\t\tEscopo (%u): ", r->scopeSize);
        for (unsigned j = 0; j < r->scopeSize; j++)
            DPRINT("%u ", r->scope[j]);
        DPRINT("\n");

        DPRINT("\t\tTuplas (%u): ", r->tupleQty);
        for (unsigned j = 0; j < r->tupleQty; j++) {
            unsigned tupleSize{(unsigned) r->tuples[j].size()};
            DPRINT("(");

            for (unsigned k = 0; k < tupleSize - 1; k++)
                DPRINT("%d ", r->tuples[j][k]);

            DPRINT("%d) ", r->tuples[j][tupleSize - 1]);
        }

        DPRINT("\n");
    }

    return;
}
