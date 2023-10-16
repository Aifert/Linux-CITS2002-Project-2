#ifndef MYSYNC_MAIN_H
#define MYSYNC_MAIN_H

#include "flags.h"
#include "processfile.h"

int exists(const char *path);

extern int sync_directories(struct FILEINFO *file_info, int file_count, const char *src, const char *dest, FLAG *flags);

#endif // MYSYNC_MAIN_H
