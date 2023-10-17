#ifndef MYSYNC_H
#define MYSYNC_H

#include "sync.h"
#include "processfile.h"
#include "flags.h"

struct DirectoryInfo
{
    char *dir_name;
    int entry_count;
};

void initializeDirectories(int num_dir);

void fillDir(char *argv[], FLAG *flags, int num_dir);

int maxEntry(int num_dir);

#endif