#include "backtracking.hpp"

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

int isValueValidInRestriction(
    Restriction *r,
    std::vector<int> &s,
    int scopeIdx,
    int value
) {
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
        #ifdef DEBUG
        std::cout << "\t\tChecking if assignment match tuple " << \
            i + 1 << std::endl;
        std::cout << "\t\t\tScope index: " << scopeIdx << "\n";
        #endif

        tupleMatch = 1;

        for (int j = scopeIdx; j >= 0; j--) {
            #ifdef DEBUG
            std::cout << "\t\t\tTuple value: " << r->tuples[i][j] << "\n";
            std::cout << "\t\t\tSolution index: " << r->scope[j] - 1 << "\n";
            std::cout << "\t\t\tAssignment value: " << \
                s[r->scope[j] - 1] << "\n";
            #endif

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

int isValueValid(
    Csp *csp,
    std::vector<int> &s,
    unsigned variableIdx,
    int value
) {
    // Check if value at index idx in solution s is valid
    Restriction *r{nullptr};
    int result, scopeIdx;

    #ifdef DEBUG
    std::cout << "Checking if value " << value << " in x" << \
        variableIdx + 1 << " is valid" << std::endl;
    #endif

    for (unsigned i = 0; i < csp->numRestr; i++) {
        r = csp->restrictions[i];

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

        #ifdef DEBUG
        std::cout << "\tx" << variableIdx + 1 << " is present in " << \
            "restriction " << i + 1 << " (type " << r->type << \
            ")" << std::endl;
        #endif

        result = isValueValidInRestriction(r, s, scopeIdx, value);

        if (!result) return false;
    }

    return true;
}


int Backtracking::backtracking(
    Csp *csp,
    std::vector<int> &solution,
    unsigned i
) {
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

        if (res == 1) return 1;
    }

    return -1;
}
    
void Backtracking::runBacktracking(Csp *csp) {
    std::vector<int> solution;

    for (unsigned i = 0; i < csp->numVars; i++)
        solution.push_back(0);

    int res = backtracking(csp, solution, 0);

    if (res == -1)
        std::cout << "INVIAVEL\n";

    return;
}
