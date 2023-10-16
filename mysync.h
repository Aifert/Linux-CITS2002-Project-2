#ifndef MYSYNC_H
#define MYSYNC_H

#include "sync.h"
#include "processfile.h"
#include "flags.h"

#define MAX_DIRECTORIES 4

struct
{
    char *dir_name;
    int entry_count;
} directories[MAX_DIRECTORIES];

void fillDir(char *argv[], FLAG *flags, int num_dir);

int maxEntry(int num_dir);

#endif