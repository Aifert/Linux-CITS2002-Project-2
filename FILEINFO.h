#ifndef FILEINFO_H
#define FILEINFO_H

#include "headers.h"

#define MAX_FILENAME_LENGTH 512
#define MAX_BUFFER_LENGTH 1024

struct FILEINFO
{
    char *filename;
    time_t modification_time;
    long long size;
};

#endif