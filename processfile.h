#ifndef PROCESSFLAGS_H
#define PROCESSFLAGS_H

#include "flags.h"
#include "glob.h"

/**
 * Check if the supplied file path exists
 *
 * @param file_path A pointer to the path name of file
 * @param size      Size of file
 *
 * @return int
 */
int match(const char *file_path, long long size);

/**
 * Search and Match the filename and size provided in destination directory
 *
 * @param dest_dir A pointer to the name of destination directory
 * @param filename A pointer to the name of file
 * @param size     Size of the file
 *
 * @return int
 */
int searchANDmatch(const char *dest_dir, const char *filename, long long size);

/**
 * Filter the entries in the source directory to find out which we have to sync into destination
 * directory
 *
 * @param src_dir A pointer to the name of source directory
 * @param dest_dir A pointer to the name of destination directory
 * @param flags A pointer to retrieve information of flags to be processed
 * @param total_count A pointer to total count of entries to sync
 *
 * @return FILEINFO
 */
extern struct FILEINFO *process_file(const char *src_dir, const char *dest_dir, FLAG *flags, int *total_count);

#endif // PROCESSFLAGS_H