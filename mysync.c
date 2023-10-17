#include "mysync.h"

int DIR_COUNT = 0;  // Initialise global DIR_COUNT variable
int FILE_COUNT = 0; // Initialise global FILE_COUNT variable

/**
 * CITS2002 Project 2
 * @author Aifert Yet 23455873
 *
 * The goal of this project is to design and develop a command-line utility program,
 * named mysync, to synchronise the contents of two or more directories.
 */
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s src_dir dest_dir\n", argv[0]);
        return 1;
    }

    FLAG *flags = checkflag(argc, argv);

    int num_dir = argc - optind;
    int x = 0;

    struct DIRECTORYINFO *directories = NULL; // Initialise array to hold directories given at command line

    initializeDirectories(&directories, num_dir); // Allocate memory to directories struct

    fillDir(argv, directories, flags, num_dir); // Fill directories struct

    if (flags->flag_v || flags->flag_n)
    {
        printf("Created space to sync %i directories. \n", num_dir); // Verbose output
    }

    int max_index = maxEntry(directories, num_dir); // Find index of max entry

    while (num_dir != 0)
    {
        if (x == max_index) // x is the index of max entry
        {
            x++;
            num_dir--;
            continue;
        }

        char *src_dir = directories[max_index].dir_name; // Make pointer to name of source directory
        char *dest_dir = directories[x].dir_name;        // Make pointer to name of destination directory

        if (flags->flag_v || flags->flag_n)
        {
            printf("---------------------------------------------------\n");
            printf("Initializing Synchronization for %s......\n", dest_dir);
            printf("---------------------------------------------------\n");
            printf("Source : '%s' , Destination : '%s'\n", src_dir, dest_dir);
            printf("Comparing top level '%s' to '%s'\n", src_dir, dest_dir);
        }

        int total_count = 0;
        struct FILEINFO *file_info = process_file(src_dir, dest_dir, flags, &total_count);

        int completion;
        completion = sync_directories(file_info, total_count, src_dir, dest_dir, flags);

        // Synced successfully
        if (completion == 0)
        {
            if (flags->flag_v || flags->flag_n)
            {
                printf("%i files, %i directories.\n", FILE_COUNT, DIR_COUNT);
            }
            printf("Synchronization complete.\n");
        }
        x++;
        num_dir--;
        DIR_COUNT = 0;  // Reset
        FILE_COUNT = 0; // Reset
    }

    cleanDirectories(directories); // Free allocated memory
    free(flags);
    return 0;
}
