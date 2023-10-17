#ifndef GLOB_H
#define GLOB_H

#include "headers.h"
#include "FLAG.h"

char *glob2regex(char *glob);

extern int matchRegex(FLAG *flags, const char *filename, int mode);

#endif // GLOB_H
