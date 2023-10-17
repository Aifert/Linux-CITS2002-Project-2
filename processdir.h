#ifndef PROCESSDIR_H
#define PROCESSDIR_H

#include "DIRECTORYINFO.h" // Include this before using DirectoryInfo
#include "FLAG.h"

/**
 * Get the number of entries in specified directory
 *
 * @param dir Pointer to name of directory
 *
 * @return int
 */
int getEntryCount(const char *dir);

/**
 * Allocate memory for directories to hold directory info
 *
 * @param directories pointer to struct that stores directory information
 * @param num_dir The number of directories given at command line
 *
 * @return void
 */
void initializeDirectories(struct DIRECTORYINFO **directories, int num_dir);

/**
 * Clear memory alloated for directories array
 *
 * @param directories pointer to struct that stores directory information
 *
 * @return void
 */
void cleanDirectories(struct DIRECTORYINFO *directories);

/**
 * Fill directories array with information provided at command line
 *
 * @param argv Arguments given at command line
 * @param directories pointer to struct that stores directory information
 * @param flags pointer to to be processed flags
 * @param num_dir Number of directories given at command line
 */
void fillDir(char *argv[], struct DIRECTORYINFO *directories, FLAG *flags, int num_dir);

/**
 * Find the index of the directory with the most entries
 *
 * @param directories pointer to struct that stores directory information
 * @param num_dir Number of directories given at command line
 *
 * @return int
 */
int maxEntry(struct DIRECTORYINFO *directories, int num_dir);

#endif
