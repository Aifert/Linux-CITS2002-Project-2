#include "processdir.h"

/**
 * Get the number of entries in specified directory
 *
 * @param dir Pointer to name of directory
 *
 * @return int
 */
int getEntryCount(const char *dir)
{
    int file_count = 0;
    DIR *dirp = opendir(dir);
    struct dirent *dp;

    if (dirp == NULL)
    {
        perror("CANNOT OPEN DIRECTORY");
        printf("HOW MANY TIMES HAVE I TOLD YOU DOUBLE CHECK BEFORE GIVING ME DIRECTORY!!!\n");
        return 1;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        file_count += 1;
    }

    closedir(dirp);
    return file_count;
}

/**
 * Allocate memory for directories to hold directory info
 *
 * @param directories pointer to struct that stores directory information
 * @param num_dir The number of directories given at command line
 *
 * @return void
 */
void initializeDirectories(struct DIRECTORYINFO **directories, int num_dir)
{
    *directories = (struct DIRECTORYINFO *)malloc(num_dir * sizeof(struct DIRECTORYINFO)); // Dynamically allocate memory for directories

    if (*directories == NULL)
    {
        printf("MEMORY ALLOCATION FAILED. \n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Clear memory alloated for directories array
 *
 * @param directories pointer to struct that stores directory information
 *
 * @return void
 */
void cleanDirectories(struct DIRECTORYINFO *directories)
{
    free(directories);
}

/**
 * Fill directories array with information provided at command line
 *
 * @param argv Arguments given at command line
 * @param directories pointer to struct that stores directory information
 * @param flags pointer to to be processed flags
 * @param num_dir Number of directories given at command line
 */
void fillDir(char *argv[], struct DIRECTORYINFO *directories, FLAG *flags, int num_dir)
{
    for (int i = 0; i < num_dir; i++)
    {
        directories[i].dir_name = argv[optind + i];
        directories[i].entry_count = getEntryCount(argv[optind + i]);
    }
}

/**
 * Find the index of the directory with the most entries
 *
 * @param directories pointer to struct that stores directory information
 * @param num_dir Number of directories given at command line
 *
 * @return int
 */
int maxEntry(struct DIRECTORYINFO *directories, int num_dir)
{
    int max_entry = 0;
    int max_index = 0;

    for (int i = 0; i < num_dir; i++)
    {
        if (directories[i].entry_count >= max_entry)
        {
            max_entry = directories[i].entry_count;
            max_index = i; // Store index
        }
    }

    return max_index;
}
