#ifndef BACKTRACKING_HPP
#define BACKTRACKING_HPP

#include "csp.hpp"

namespace csp {

class Backtracking {
    public:
        Backtracking() = delete; // No instantiation

        virtual ~Backtracking() = default;

        static void runBacktracking(Csp *csp);

    private:
        static bool backtracking(Csp *csp, std::vector<int> &solution,
                                 unsigned i, unsigned numVarsAssigned,
                                 std::vector<bool> &varsAssigned);
};

} // namespace csp

#endif
