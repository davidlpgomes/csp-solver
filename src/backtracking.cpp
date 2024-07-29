#include "backtracking.hpp"
#include "debug.hpp"

#include <iostream>
#include <limits>
#include <stack>

using namespace csp;

bool reviewRestrictionGAC(Csp *csp, Restriction *r, unsigned xi) {
    if (r->type == 0)
        return 0;

    DPRINT("[GAC-3] Reviewing restriction %u for variable %u\n", r->numRestr,
           xi + 1);
    unsigned xiScopeIdx{r->varsIdxAtTuple[xi + 1]};
    unsigned dSize = csp->domains[xi].size();

    bool foundMatch, changedDomain = false;

    for (unsigned di = 0; di < dSize; di++) {
        if (!csp->domainsIdxValid[xi][di])
            continue;

        DPRINT("[GAC-3] \tChecking if value %d is valid for variable x%u\n",
               csp->domains[xi][di], xi + 1);

        foundMatch = false;

        for (unsigned ti = 0; ti < r->tupleQty; ti++) {
            if (r->tuplesInvalidCount[ti] > 0) {
                DPRINT("[GAC-3] \t\tTuple %u is invalid\n", ti + 1);
                continue;
            }

            if (csp->domains[xi][di] == r->tuples[ti][xiScopeIdx]) {
                DPRINT(
                    "[GAC-3] \t\tTuple %u is valid, restriction is satisfied\n",
                    ti + 1);
                foundMatch = true;
                break;
            }
        }

        if (foundMatch)
            continue;

        DPRINT("[GAC-3] \tRemoving value %d from x%u domain\n",
               csp->domains[xi][di], xi + 1);
        csp->removeValueFromVarDomain(xi, di);
        changedDomain = true;
    }

    return changedDomain;
}

void gac3(Csp *csp) {
    DPRINT("Running GAC-3\n");
    std::stack<std::pair<unsigned, Restriction *>> restrStack;

    Restriction *r, *r2;

    for (unsigned i = 0; i < csp->numRestr; i++) {
        r = csp->restrictions[i];

        if (r->type == 0)
            continue;

        for (unsigned j = 0; j < r->scopeSize; j++) {
            DPRINT("[GAC-3] Adding restriction %d for x%d\n", r->numRestr,
                   r->scope[j]);
            restrStack.emplace(r->scope[j] - 1, r);
        }
    }

    std::pair<unsigned, Restriction *> restPair;
    unsigned xi, xj;

    while (restrStack.size() > 0) {
        restPair = restrStack.top();
        restrStack.pop();

        xi = restPair.first;
        r = restPair.second;

        if (!reviewRestrictionGAC(csp, r, xi))
            continue;

        // Add other restrictions that have xi
        unsigned xiRestsQnty = csp->variablesRestrictions[xi].size();
        for (unsigned i = 0; i < xiRestsQnty; i++) {
            r2 = csp->variablesRestrictions[xi][i].second;

            if (r2->type == 0)
                continue;

            for (unsigned j = 0; j < r2->scopeSize; j++) {
                xj = r2->scope[j] - 1;

                if (xj == xi)
                    continue;

                restrStack.emplace(xj, r2);
                DPRINT("[GAC-3] Adding restriction %d for x%d\n", r2->numRestr,
                       r2->scope[j]);
            }
        }
    }

    DPRINT("[GAC-3] Domains after GAC-3\n");
    for (unsigned i = 0; i < csp->numVars; i++) {
        unsigned dSize = csp->domains[i].size();

        DPRINT("[GAC-3] \tx%u: ", i + 1);

        for (unsigned j = 0; j < dSize; j++) {
            if (!csp->domainsIdxValid[i][j])
                continue;

            DPRINT("%d ", csp->domains[i][j]);
        }

        DPRINT("\n");
    }

    return;
}

bool isValueValidInRestriction(Restriction *r, std::vector<int> &s) {
    bool matchedAnyTuple = false;
    bool tupleMatch;

    for (unsigned i = 0; i < r->tupleQty; i++) {
        DPRINT("\t\tChecking if assignment match tuple %u\n", i + 1);

        tupleMatch = true;

        for (unsigned j = 0; j < r->scopeSize; j++) {
            DPRINT("\t\t\tTuple value: %d\n", r->tuples[i][j]);
            DPRINT("\t\t\tSolution index: %d\n", r->scope[j] - 1);
            DPRINT("\t\t\tAssignment value: %d\n", s[r->scope[j] - 1]);

            if (r->tuples[i][j] != s[r->scope[j] - 1]) {
                tupleMatch = false;
                break;
            }
        }

        // Tuple doesn't match values assignment, next tuple...
        if (!tupleMatch) {
            DPRINT("\t\tDid not matched tuple\n");
            continue;
        }

        DPRINT("\t\tMatched tuple %u\n", i + 1);
        matchedAnyTuple = true;
        break;
    }

    return !(matchedAnyTuple ^ r->type);
}

bool isValueValid(Csp *csp, std::vector<int> &s, unsigned variableIdx,
                  unsigned numVarsAssigned, std::vector<bool> &varsAssigned) {
    // Check if value at index idx in solution s is valid
    Restriction *r{nullptr};

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

        if (!isValueValidInRestriction(r, s))
            return false;

        i++;
    }

    return true;
}

int getSmallestDomainVarIdx(Csp *csp, std::vector<bool> &varsAssigned) {
    // Return the index of the variable with the smallest (valid) domain that is
    // not assigned yet.
    unsigned smallestDomain{std::numeric_limits<unsigned>::max()};
    int smallestDomainVarIdx = -1;

    for (unsigned i = 0; i < csp->numVars; i++) {
        if (varsAssigned[i] || csp->domainsValidSize[i] >= smallestDomain)
            continue;

        smallestDomain = csp->domainsValidSize[i];
        smallestDomainVarIdx = i;
    }

    DPRINT("Variable with smallest domain is x%d, size: %u\n",
           smallestDomainVarIdx + 1, smallestDomain);

    return smallestDomainVarIdx;
}

bool Backtracking::backtracking(Csp *csp, std::vector<int> &solution,
                                unsigned i, unsigned numVarsAssigned,
                                std::vector<bool> &varsAssigned) {
    numVarsAssigned++;
    varsAssigned[i] = true;

    std::vector<int> &d{csp->domains[i]};
    unsigned dSize = d.size();

    for (unsigned di = 0; di < dSize; di++) {
        if (!csp->domainsIdxValid[i][di])
            continue;

        solution[i] = d[di];

        if (!isValueValid(csp, solution, i, numVarsAssigned, varsAssigned))
            continue;

        // Solution found
        if (numVarsAssigned == csp->numVars) {
            for (unsigned j = 0; j < csp->numVars; j++)
                std::cout << "x" << j + 1 << " = " << solution[j] << std::endl;

            return true;
        }

        csp->removeValueFromVarDomain(i, di);
        gac3(csp);

        int nextVarIdx = getSmallestDomainVarIdx(csp, varsAssigned);

        if (backtracking(csp, solution, nextVarIdx, numVarsAssigned,
                         varsAssigned))
            return true;
    }

    csp->resetVarDomain(i);

    numVarsAssigned--;
    varsAssigned[i] = false;

    return false;
}

void Backtracking::runBacktracking(Csp *csp) {
    std::vector<int> solution(csp->numVars, 0);
    std::vector<bool> varsAssigned(csp->numVars, false);

    gac3(csp);

    int initVarIdx = getSmallestDomainVarIdx(csp, varsAssigned);
    DPRINT("Variable with smallest domain: x%d\n", initVarIdx + 1);

    if (!backtracking(csp, solution, initVarIdx, 0, varsAssigned))
        std::cout << "INVIAVEL\n";

    return;
}
