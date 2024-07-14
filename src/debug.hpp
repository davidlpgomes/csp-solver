#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cstdio>

namespace csp {

#ifdef DEBUG
    #define D(x) x
    #define DPRINT(...)                                                        \
        do {                                                                   \
            fprintf(stderr, __VA_ARGS__);                                      \
        } while (0)
#else
    #define D(x)
    #define DPRINT(...)
#endif

} // namespace csp

#endif
