#ifndef NNG_SUPPLEMENTAL_UTIL_OPTIONS_H
#define NNG_SUPPLEMENTAL_UTIL_OPTIONS_H

#include <stdbool.h>

struct nng_optspec {
    char const * o_name;
    int          o_short;
    int          o_val;
    bool         o_arg;
};

typedef struct nng_optspec nng_optspec;

#endif // NNG_SUPPLEMENTAL_UTIL_OPTIONS_H
