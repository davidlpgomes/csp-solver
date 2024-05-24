#include "csp.hpp"

#include <iostream>

using namespace csp;

Csp::Csp() {};

Csp::~Csp() {
    std::vector<Restriction*>::iterator it{this->restrictions.begin()};

    for (; it != this->restrictions.end(); ++it)
        if (*it != nullptr) delete *it;
}

Csp* Csp::fromInput() {
    Csp *csp{new Csp};

    std::cin >> csp->numVars;

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
    for (unsigned i = 0; i < csp->numRestr; i++) {
        r = Restriction::fromInput(); 
        csp->restrictions.push_back(r);
    }

    return csp;
}

void Csp::print() {
    std::cout << "Número de variáveis: " << this->numVars << std::endl;

    for (unsigned i = 0; i < this->numVars; i++) {
        std::set<int>::iterator it{this->domains[i].begin()};

        std::cout << "\tDomínio x" << i + 1 << ": ";

        for (; it != this->domains[i].end(); ++it)
            std::cout << *it << " ";

        std::cout << "\n";
    }

    std::cout << "Número de restrições: " << this->numRestr << std::endl;

    Restriction *r;
    for (unsigned i = 0; i < this->numRestr; i++) {
        r = this->restrictions[i];
        std::cout << "\tRestrição " << i + 1 << ":" << std::endl;

        std::cout << "\t\tTipo: ";
        if (!r->type) std::cout << "valores inválidos";
        else std::cout << "valores válidos";
        std::cout << std::endl;

        std::cout << "\t\tEscopo (" << r->scopeSize << "): ";
        for (unsigned j = 0; j < r->scopeSize; j++)
            std::cout << r->scope[j] << " ";
        std::cout << std::endl;


        std::cout << "\t\tTuplas (" << r->tupleQty << "): ";
        for (unsigned j = 0; j < r->tupleQty; j++) {
            unsigned tupleSize{(unsigned) r->tuples[j].size()};
            std::cout << "(";

            for (unsigned k = 0; k < tupleSize - 1; k++)
                std::cout << r->tuples[j][k] << " ";

            std::cout << r->tuples[j][tupleSize - 1] << ") ";
        }

        std::cout << std::endl;
    }

    return;
}
