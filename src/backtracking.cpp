#include "backtracking.hpp"

#include <iostream>

using namespace csp;


int isValueValid(
    Csp *csp,
    std::vector<Restriction*> &res,
    std::vector<int> &s,
    int value,
    int idx
) {
    // Check if value at index idx in solution s is valid
    for (int i = 0; i < csp->numRestr; i++) {

    }
}


int Backtracking::backtracking(
    Csp *csp,
    std::vector<int> &solution,
    unsigned i
) {
    if (i == csp->numVars) {
        for (unsigned j = 0; j < csp->numVars; j++)
            std::cout << solution[j] << " ";
        std::cout << "\n";

        return -1;
    }

    // Domain
    std::set<int> &d{csp->domains[i]};
    std::set<int>::iterator it{d.begin()};

    for (; it != d.end(); ++it) {
        solution[i] = *it;
        backtracking(csp, solution, i + 1);
    }

    return 1;
}
    
void Backtracking::runBacktracking(Csp *csp) {
    std::vector<int> solution;

    for (unsigned i = 0; i < csp->numVars; i++)
        solution.push_back(0);

    backtracking(csp, solution, 0);

    return;
}
