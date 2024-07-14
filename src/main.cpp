#include "backtracking.hpp"
#include "csp.hpp"
#include "debug.hpp"

using namespace csp;

int main() {
    Csp *csp{Csp::fromInput()};

    DPRINT("INFO: Running in DEBUG mode.\n");
    DPRINT("\n---------- PROBLEMA:\n");
    D(csp->print());
    DPRINT("---------------------\n");

    // Backtracking
    Backtracking::runBacktracking(csp);

    delete csp;

    return 0;
}
