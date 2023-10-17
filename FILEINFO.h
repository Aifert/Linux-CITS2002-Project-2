#ifndef FILEINFO_H
#define FILEINFO_H

#include "headers.h"

struct FILEINFO
{
    char *filename;           //  Filename to be synced
    time_t modification_time; //  Modification time of file to be synced
    long long size;           //  Size of file to be synced
    int newer;                //  1 if newest, 0 if not
};

#endif