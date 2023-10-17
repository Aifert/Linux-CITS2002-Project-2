#ifndef FLAG_H
#define FLAG_H

#include "headers.h"

typedef struct
{
    int flag_a;          // (1) flag a is enabled
    int flag_i;          // (1) flag i is enabled
    int pattern_i_count; // number of patterns given for flag i
    char **patterns_i;   // array to store patterns for flag i
    int flag_n;          // (1) flag n is enabled
    int flag_o;          // (1) flag o is enabled
    int pattern_o_count; // number of patterns given for flag i
    char **patterns_o;   // array to store patterns for flag i
    int flag_r;          // (1) flag r is enabled
    int flag_v;          // (1) flag v is enabled,
    int flag_p;          // (1) flag p is enabled
} FLAG;                  // FLAG data type, stores all the flags from command line

#endif // FLAG_H