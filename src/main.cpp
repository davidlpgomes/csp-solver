#include <iostream>

#include "backtracking.hpp"
#include "csp.hpp"

using namespace csp;

int main() {
    Csp *csp{Csp::fromInput()};

    #ifdef DEBUG
    std::cout << "INFO: Running in DEBUG mode." << std::endl;

    std::cout << "\n---------- PROBLEMA:" << std::endl;
    csp->print();
    std::cout << "---------------------" << std::endl;
    #endif

    // Backtracking
    Backtracking::runBacktracking(csp);

    delete csp;

    return 0;
}
