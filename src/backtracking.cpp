#include "backtracking.hpp"
#include "debug.hpp"

#include <iostream>

using namespace csp;

int getVariableIdxAtRestrScope(Restriction *r, unsigned variableIdx) {
    // Returns the index of the variable in the restriction
    // Returns -1 if the variable is not present in the restriction
    variableIdx++;

    for (unsigned i = 0; i < r->scopeSize; i++)
        if (r->scope[i] == variableIdx)
            return i;

    return -1;
}

int isValueValidInRestriction(Restriction *r, std::vector<int> &s, int scopeIdx,
                              int value) {
    int matchedAnyTuple = 0;

    if (scopeIdx == 0 && r->scopeSize == 1) {
        for (unsigned i = 0; i < r->tupleQty; i++) {
            if (value == r->tuples[i][0]) {
                matchedAnyTuple = 1;
                break;
            }
        }

        return !(matchedAnyTuple ^ r->type);
    }

    int tupleMatch;

    for (unsigned i = 0; i < r->tupleQty; i++) {
        DPRINT("\t\tChecking if assignment match tuple %u\n", i + 1);
        DPRINT("\t\t\tScope index: %u\n", scopeIdx);

        tupleMatch = 1;

        for (int j = scopeIdx; j >= 0; j--) {
            DPRINT("\t\t\tTuple value: %d\n", r->tuples[i][j]);
            DPRINT("\t\t\tSolution index: %d\n", r->scope[j] - 1);
            DPRINT("\t\t\tAssignment value: %d\n", s[r->scope[j] - 1]);

            if (r->tuples[i][j] != s[r->scope[j] - 1]) {
                tupleMatch = 0;
                break;
            }
        }

        // Tuple doesn't match values assignment, next tuple...
        if (!tupleMatch)
            continue;

        matchedAnyTuple = 1;
        break;
    }

    return !(matchedAnyTuple ^ r->type);
}

int isValueValid(Csp *csp, std::vector<int> &s, unsigned variableIdx,
                 int value) {
    // Check if value at index idx in solution s is valid
    Restriction *r{nullptr};
    int result, scopeIdx;

    DPRINT("Checking if value %d in x%u is valid\n", value, variableIdx + 1);

    std::vector<Restriction *>::iterator it =
        csp->variablesRestrictions[variableIdx].begin();

    unsigned i = 0;
    for (; it != csp->variablesRestrictions[variableIdx].end(); ++it) {
        r = *it;

        /*
        scopeIdx = getVariableIdxAtRestrScope(r, variableIdx);

        // Restriction does not apply to variable (-1) or
        // scope index is 0, therefore can't check if restriction is valid
        if (scopeIdx == -1 || (r->scopeSize > 1 && scopeIdx == 0))
            continue;
        */

        scopeIdx = r->scopeSize - 1;

        // Variable is not the last one in the scope, skip
        if (r->scope[scopeIdx] != variableIdx + 1)
            continue;

        DPRINT("\tx%u is present in restriction %u (type %d)\n",
               variableIdx + 1, i + 1, r->type);

        result = isValueValidInRestriction(r, s, scopeIdx, value);

        if (!result)
            return false;

        i++;
    }

    return true;
}

int Backtracking::backtracking(Csp *csp, std::vector<int> &solution,
                               unsigned i) {
    if (i == csp->numVars) {
        // Found solution
        for (unsigned j = 0; j < csp->numVars; j++)
            std::cout << "x" << j + 1 << " = " << solution[j] << std::endl;

        return 1;
    }

    // Domain
    std::set<int> &d{csp->domains[i]};
    std::set<int>::iterator it{d.begin()};
    int res;

    for (; it != d.end(); ++it) {
        // Check if the atrribution follows the restrictions
        solution[i] = *it;

        if (!isValueValid(csp, solution, i, *it))
            continue;

        res = backtracking(csp, solution, i + 1);

        if (res == 1)
            return 1;
    }

    return -1;
}

void Backtracking::runBacktracking(Csp *csp) {
    std::vector<int> solution(csp->numVars, 0);

    int res = backtracking(csp, solution, 0);

    if (res == -1)
        std::cout << "INVIAVEL\n";

    return;
}
