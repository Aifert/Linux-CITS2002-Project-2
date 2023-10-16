#ifndef PROCESSFLAGS_H
#define PROCESSFLAGS_H

#include "flags.h"
#include "glob.h"

int match(const char *file_path, long long size);

int searchANDmatch(const char *dest_dir, const char *filename, long long size);

int getEntryCount(const char *dir, FLAG *flags);

// PROCESS A FLAG
// The -a option requests that all files, regardless of whether they begin with a '.' or not,
// should be considered for synchronisation.
extern struct FILEINFO *process_file(const char *src_dir, const char *dest_dir, FLAG *flags, int *file_count);

#endif // PROCESSFLAGS_H