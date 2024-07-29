#include "csp.hpp"
#include "debug.hpp"

#include <algorithm>
#include <iostream>

using namespace csp;

Csp::Csp() {};

Csp::~Csp() {
    std::vector<Restriction *>::iterator it{this->restrictions.begin()};

    for (; it != this->restrictions.end(); ++it)
        if (*it != nullptr)
            delete *it;
}

Csp *Csp::fromInput() {
    Csp *csp{new Csp};

    std::cin >> csp->numVars;

    unsigned domainSize;
    for (unsigned i = 0; i < csp->numVars; i++) {
        std::cin >> domainSize;
        std::vector<int> domain(domainSize);
        std::vector<bool> domainValid(domainSize, true);

        std::map<int, unsigned> domainIdxs;
        csp->domainsIdxs.push_back(domainIdxs);

        for (unsigned j = 0; j < domainSize; j++) {
            std::cin >> domain[j];
            csp->domainsIdxs[i][domain[j]] = j;
        }

        // Needs to be sorted for reviewRestrictionGAC
        std::sort(domain.begin(), domain.end());

        csp->domains.push_back(domain);
        csp->domainsValidSize.push_back(domainSize);
        csp->domainsIdxValid.push_back(domainValid);

        std::vector<std::pair<unsigned, Restriction *>> res;
        csp->variablesRestrictions.push_back(res);

        std::queue<unsigned *> q;
        csp->tuplesInvalidadedAddrs.push_back(q);
    }

    std::cin >> csp->numRestr;

    Restriction *r;
    DPRINT("Running GAC3\n");
    for (unsigned i = 0; i < csp->numRestr; i++) {
        r = Restriction::fromInput();
        r->numRestr = i + 1;
        csp->restrictions.push_back(r);

        for (unsigned j = 0; j < r->scopeSize; j++)
            csp->variablesRestrictions[r->scope[j] - 1].push_back(
                std::make_pair(j, r));
    }

    return csp;
}

void Csp::removeValueFromVarDomain(unsigned xi, unsigned di) {
    // Iterates over all tuples of the restrictions that has xi in its scope
    // that matches the 'value' in domain[di] and increment its
    // tupleInvalidCount

    unsigned restrCount = this->variablesRestrictions[xi].size();

    Restriction *r;
    unsigned scopeIdx;

    for (unsigned ri = 0; ri < restrCount; ri++) {
        scopeIdx = this->variablesRestrictions[xi][ri].first;
        r = this->variablesRestrictions[xi][ri].second;

        for (unsigned ti = 0; ti < r->tupleQty; ti++) {
            if (r->tuples[ti][scopeIdx] != this->domains[xi][di])
                continue;

            r->tuplesInvalidCount[ti]++;
            this->tuplesInvalidadedAddrs[xi].push(&r->tuplesInvalidCount[ti]);
        }
    }

    this->domainsIdxValid[xi][di] = false;
    this->domainsValidSize[xi]--;

    return;
}

void Csp::resetVarDomain(unsigned xi) {
    // Reset domain of var x<xi + 1>
    while (!this->tuplesInvalidadedAddrs[xi].empty()) {
        unsigned *tuplesInvalidadedAddr =
            this->tuplesInvalidadedAddrs[xi].front();
        this->tuplesInvalidadedAddrs[xi].pop();

        *tuplesInvalidadedAddr -= 1;
    }

    std::fill(this->domainsIdxValid[xi].begin(),
              this->domainsIdxValid[xi].end(), true);
    this->domainsValidSize[xi] = this->domains[xi].size();

    return;
}

void Csp::print() {
    DPRINT("Número de variáveis: %u\n", this->numVars);

    for (unsigned xi = 0; xi < this->numVars; xi++) {
        DPRINT("\tDomínio x%u: ", xi + 1);

        unsigned dSize = this->domains[xi].size();
        for (unsigned di = 0; di != dSize; di++) {
            if (this->domainsIdxValid[xi][di]) {
                DPRINT("%d ", this->domains[xi][di]);
            }
        }

        DPRINT("\n");
    }

    DPRINT("Número de restrições: %u\n", this->numRestr);

    Restriction *r;
    for (unsigned i = 0; i < this->numRestr; i++) {
        r = this->restrictions[i];
        DPRINT("\tRestrição %u:\n", r->numRestr);

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
