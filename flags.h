#ifndef FLAGS_H
#define FLAGS_H

#include "FLAG.h"
#include "FILEINFO.h"

#define OPTLIST "ai:no:rvp"

char *strdup(const char *str);

extern FLAG *checkflag(int argc, char *arg_value[]);

extern void printflags(FLAG *flags);

extern bool process_a(FLAG *flags);

extern void process_n(const char *src_dir, const char *dest_dir, struct FILEINFO *file_info, int file_count);

extern void process_v(FLAG *flags, int accept, const char *dest_path, struct dirent *entry);

#endif // FLAGS_H
