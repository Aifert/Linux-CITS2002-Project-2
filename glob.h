#ifndef GLOB_H
#define GLOB_H

#include "headers.h" // For header files
#include "FLAG.h"    // For FLAG data

/**
 * Convert glob pattern into a regular expression
 *
 * @param glob The pattern to be processed
 *
 * @return char* A pointer to the resulting regular expression
 */
char *glob2regex(char *glob);

/**
 * Match filename to each pattern given at command line
 *
 * @param flags Pointer to FLAG to be processed
 * @param filename Pointer to filename to be processed
 * @param mode (1) process i flag, (0) process o flag
 *
 * @return int
 */
extern int matchRegex(FLAG *flags, const char *filename, int mode);

#endif // GLOB_H
