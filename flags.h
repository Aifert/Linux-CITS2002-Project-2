#ifndef FLAGS_H
#define FLAGS_H

#define _POSIX_C_SOURCE 200809L // Included to use strdup function

#include "FLAG.h"     // For FLAG datatype
#include "FILEINFO.h" // For FILEINFO data

#define OPTLIST "ai:no:rvp" // Declared a list of OPTIONS to look out for in command line

/**
 * Initialisation of FLAG datatype
 *
 * @param agrc The number of arguments
 * @param arg_value The arguments given in command line
 * @return FLAG
 */
extern FLAG *checkflag(int argc, char *arg_value[]);

/**
 * Process n flag
 * Prints out a simulated synchronization process, but don't actually synchronize
 *
 * The -n option requests that any files to be copied are identified, but they are not actually synchronised
 *
 * @param src_dir Pointer to a char to be processed, representing source directory
 * @param dest_dir Pointer to a char to be processed, representing destination directory
 * @param file_info Pointer to a FILE INFO to be synced
 * @param file_count The number of files to be synced
 *
 * @return void
 */
extern void process_n(const char *src_dir, const char *dest_dir, struct FILEINFO *file_info, int file_count);

/**
 * Process v flag
 * Prints out if a file is accepted or not, verbose output
 *
 * The -v option requests that mysync be more verbose in its output, reporting its actions to stdout
 *
 * @param flags Pointer to Flags to be processed
 * @param entry Pointer to an entry which displays info about each file in directory
 *
 * @return void
 */
extern void process_v(FLAG *flags, int accept, struct dirent *entry);

/**
 * Print flags
 *
 * Can be used for debug output, but prints all the flags we have passed into command line argument
 *
 * @param flags Pointer to Flags to be processed
 *
 * @return void
 */
extern void printflags(FLAG *flags);

#endif // FLAGS_H
