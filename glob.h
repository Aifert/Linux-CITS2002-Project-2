#ifndef GLOB_H
#define GLOB_H

#include "headers.h"

char *glob2regex(char *glob);

extern int matchRegex(char *globPattern, const char *filename);

#endif // GLOB_H
