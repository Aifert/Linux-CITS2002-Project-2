#include "flags.h"

/**
 * Initialisation of FLAG datatype
 *
 * @param agrc The number of arguments
 * @param arg_value The arguments given in command line
 * @return FLAG
 */
FLAG *checkflag(int argc, char *arg_value[])
{
    FLAG *flags = calloc(1, sizeof(FLAG)); // Dynamically allocate required size for flags

    if (flags == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int opt;
    while ((opt = getopt(argc, arg_value, OPTLIST)) != -1) // Get list of flags declared at command line
    {
        switch (opt)
        {
        case 'a':
            flags->flag_a = 1;
            break;

        case 'i':
            flags->flag_i = 1;
            flags->patterns_i = realloc(flags->patterns_i, sizeof(char *) * (flags->pattern_i_count + 1)); // Dynamically allocate memory to anticipate more patterns
            if (flags->patterns_i == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            flags->patterns_i[flags->pattern_i_count] = strdup(optarg); // Store the argument of -i option
            flags->pattern_i_count++;                                   // Increment pattern count
            break;

        case 'n':
            flags->flag_n = 1;
            break;

        case 'o':
            flags->flag_o = 1;
            flags->patterns_o = realloc(flags->patterns_o, sizeof(char *) * (flags->pattern_o_count + 1)); // Dynamically allocate memory to anticipate more patterns
            if (flags->patterns_o == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            flags->patterns_o[flags->pattern_o_count] = strdup(optarg); // Store the argument of -o option
            flags->pattern_o_count++;                                   // Increment pattern count
            break;

        case 'r':
            flags->flag_r = 1;
            break;

        case 'v':
            flags->flag_v = 1;
            break;

        case 'p':
            flags->flag_p = 1;
            break;
        }
    }

    return flags;
}

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
void process_n(const char *src_dir, const char *dest_dir, struct FILEINFO *file_info, int file_count)
{
    char src_path[MAX_FILENAME_LENGTH];
    char dest_path[MAX_FILENAME_LENGTH];

    struct FILEINFO *current_file = file_info; // Create a pointer and points to file info

    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < file_count; i++)
    {
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, current_file->filename); // Concatenate a string in the form {directory}/{filename}, as path
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, current_file->filename);

        printf("Copy from %s to %s\n", src_path, dest_path);        // Prints simulated copying
        printf("Copying %llo bytes......\n\n", current_file->size); // Prints extra detail such as the size to copy
        current_file++;
    }

    printf("--------------------------------------------------------------------------------\n");
}

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
void process_v(FLAG *flags, int accept, struct dirent *entry)
{
    if (flags->flag_v || flags->flag_n) // Including n because v is the same as n flag just WITH synchronization
    {
        if (accept) // If the file is to be accepted process below
        {
            printf("ACCEPTING file '%s'\n", entry->d_name);
        }
        else
        {
            printf("SKIPPING file '%s'\n", entry->d_name);
        }
    }
}

/**
 * Print flags
 *
 * Can be used for debug output, but prints all the flags we have passed into command line argument
 *
 * @param flags Pointer to Flags to be processed
 *
 * @return void
 */
void printflags(FLAG *flags)
{
    printf("flag_a: %d\n", flags->flag_a);
    printf("flag_n: %d\n", flags->flag_n);
    printf("flag_r: %d\n", flags->flag_r);
    printf("flag_v: %d\n", flags->flag_v);
    printf("flag_p: %d\n", flags->flag_p);
    printf("flag_i: %d\n", flags->flag_i);
    printf("flag_o: %d\n", flags->flag_o);

    for (int i = 0; i < flags->pattern_i_count; i++)
    {
        printf("pattern_i[%d]: %s\n", i, flags->patterns_i[i]);
    }

    for (int i = 0; i < flags->pattern_o_count; i++)
    {
        printf("pattern_o[%d]: %s\n", i, flags->patterns_o[i]);
    }
}
