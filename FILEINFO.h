#ifndef FILEINFO_H
#define FILEINFO_H

#include "headers.h"

struct FILEINFO
{
    char *filename;
    time_t modification_time;
    long long size;
    int newer;
};

#endif