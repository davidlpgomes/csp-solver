#include "backtracking.hpp"
#include "debug.hpp"

#include <iostream>

using namespace csp;

int isValueValidInRestriction(Restriction *r, std::vector<int> &s) {
    int matchedAnyTuple = 0;
    int tupleMatch;

    for (unsigned i = 0; i < r->tupleQty; i++) {
        DPRINT("\t\tChecking if assignment match tuple %u\n", i + 1);

        tupleMatch = 1;

        for (unsigned j = 0; j < r->scopeSize; j++) {
            DPRINT("\t\t\tTuple value: %d\n", r->tuples[i][j]);
            DPRINT("\t\t\tSolution index: %d\n", r->scope[j] - 1);
            DPRINT("\t\t\tAssignment value: %d\n", s[r->scope[j] - 1]);

            if (r->tuples[i][j] != s[r->scope[j] - 1]) {
                tupleMatch = 0;
                break;
            }
        }

        // Tuple doesn't match values assignment, next tuple...
        if (!tupleMatch) {
            DPRINT("\t\tDid not matched tuple\n");
            continue;
        }

        DPRINT("\t\tMatched tuple %u\n", i + 1);
        matchedAnyTuple = 1;
        break;
    }

    return !(matchedAnyTuple ^ r->type);
}

int isValueValid(Csp *csp, std::vector<int> &s, unsigned variableIdx,
                 unsigned numVarsAssigned, std::vector<bool> &varsAssigned) {
    // Check if value at index idx in solution s is valid
    Restriction *r{nullptr};
    int result;

    DPRINT("Checking if value %d in x%u is valid\n", s[variableIdx],
           variableIdx + 1);

    std::vector<std::pair<unsigned, Restriction *>>::iterator it =
        csp->variablesRestrictions[variableIdx].begin();

    unsigned i = 0;
    for (; it != csp->variablesRestrictions[variableIdx].end(); ++it) {
        r = it->second;

        // Check if all restriction's scope variables are assigned
        if (numVarsAssigned < r->scopeSize)
            continue;

        std::vector<unsigned>::iterator itS{r->scope.begin()};
        bool allScopeVarsAssigned = true;
        for (; itS != r->scope.end(); ++itS) {
            if (!varsAssigned[*itS - 1]) {
                allScopeVarsAssigned = false;
                break;
            }
        }

        if (!allScopeVarsAssigned)
            continue;

        DPRINT("\tx%u is present in restriction %u (type %d)\n",
               variableIdx + 1, r->numRestr, r->type);

        result = isValueValidInRestriction(r, s);

        if (!result)
            return false;

        i++;
    }

    return true;
}

int Backtracking::backtracking(Csp *csp, std::vector<int> &solution, unsigned i,
                               unsigned numVarsAssigned,
                               std::vector<bool> &varsAssigned) {
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
        solution[i] = *it;

        numVarsAssigned++;
        varsAssigned[i] = true;

        if (!isValueValid(csp, solution, i, numVarsAssigned, varsAssigned)) {
            numVarsAssigned--;
            varsAssigned[i] = false;
            continue;
        }

        res = backtracking(csp, solution, i + 1, numVarsAssigned, varsAssigned);

        numVarsAssigned--;
        varsAssigned[i] = false;

        if (res == 1)
            return 1;
    }

    return -1;
}

void Backtracking::runBacktracking(Csp *csp) {
    std::vector<int> solution(csp->numVars, 0);
    std::vector<bool> varsAssigned(csp->numVars, false);

    int res = backtracking(csp, solution, 0, 0, varsAssigned);

    if (res == -1)
        std::cout << "INVIAVEL\n";

    return;
}
