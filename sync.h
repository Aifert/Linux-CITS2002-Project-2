#ifndef MYSYNC_MAIN_H
#define MYSYNC_MAIN_H

#include "flags.h"
#include "processfile.h"

/**
 * Check if a sub directory already exist
 *
 * @param path A pointer to the name of subdirectory
 *
 * @return int
 */
int exists(const char *path);

/**
 * Function to sync a directory recursively if encounters subdirectories
 *
 * @param file_info A pointer to struct FILEINFO that contains information about file/directory to sync
 * @param total_count The total number of files/directories to be synced
 * @param src       A pointer to the name of source directory
 * @param dest      A pointer to the name of destination directory
 * @param flags     A pointer to FLAG to retrieve information of flags to be processed
 *
 * return int
 */
extern int sync_directories(struct FILEINFO *file_info, int file_count, const char *src, const char *dest, FLAG *flags);

#endif // MYSYNC_MAIN_H
