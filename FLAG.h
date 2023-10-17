#ifndef FLAG_H
#define FLAG_H

#include "headers.h"

typedef struct
{
    int flag_a;
    int flag_i;
    int pattern_i_count;
    char **patterns_i;
    int flag_n;
    int flag_o;
    int pattern_o_count;
    char **patterns_o;
    int flag_r;
    int flag_v;
    int flag_p;
} FLAG;

#endif // FLAG_H