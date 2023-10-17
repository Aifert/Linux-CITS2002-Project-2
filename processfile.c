#include "processfile.h"

/**
 * Check if the supplied file path exists
 *
 * @param file_path A pointer to the path name of file
 * @param size      Size of file
 *
 * @return int
 */
int match(const char *file_path, long long size)
{
    struct stat buffer;

    if (stat(file_path, &buffer) != 0)
    {
        return 0; // File does not exist
    }

    // Check if the size and modification time match
    if (size == buffer.st_size)
    {
        return 1; // File exists in the destination directory and matches all criteria
    }

    return 0; // File exists in the destination directory but does not match the criteria
}

/**
 * Search and Match the filename and size provided in destination directory
 *
 * @param dest_dir A pointer to the name of destination directory
 * @param filename A pointer to the name of file
 * @param size     Size of the file
 *
 * @return int
 */
int searchANDmatch(const char *dest_dir, const char *filename, long long size)
{
    DIR *dir;
    struct dirent *entry;
    char file_path[MAX_FILENAME_LENGTH];

    if ((dir = opendir(dest_dir)) == NULL)
    {
        printf("ERROR AT SEARCHANDMATCH");
        perror("ERROR OPENING DESTINATION DIRECTORY");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        snprintf(file_path, sizeof(file_path), "%s/%s", dest_dir, filename); // Concatenate and form a string in the form of {directory}/{filename}, creating a filepath

        if (match(file_path, size))
        {
            return 1; // There exists a match in destination directory
        }
    }

    closedir(dir);
    return 0;
}

/**
 * Check which file is newer, source or destination
 *
 * @param src_path A pointer to the name of source path
 * @param dest_path A pointer to the name of destination path
 *
 * @return int
 */
int checkTime(const char *src_path, const char *dest_path)
{
    struct stat src_statbuf;
    struct stat dest_statbuf;

    if (stat(src_path, &src_statbuf) == -1)
    {
        printf("Error at Checktime\n");
        perror("ERROR GETTING SOURCE FILE INFORMATION");
        return 0;
    }

    if (stat(dest_path, &dest_statbuf) == -1)
    {
        printf("Error at Checktime\n");
        perror("ERROR GETTING DEST FILE INFORMATION");
        return 0;
    }

    // Destination is newer than source
    if (dest_statbuf.st_mtime > src_statbuf.st_mtime)
    {
        return 3;
    }
    // Destination is older than source
    else if (dest_statbuf.st_mtime < src_statbuf.st_mtime)
    {
        return 2;
    }
    // Destination and Source is equal
    else
    {
        return 1;
    }
}

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
struct FILEINFO *process_file(const char *src_dir, const char *dest_dir, FLAG *flags, int *total_count)
{
    DIR *dir;
    struct dirent *entry;
    struct FILEINFO *file_info = NULL;

    char src_path[MAX_FILENAME_LENGTH];  // Declare source path
    char dest_path[MAX_FILENAME_LENGTH]; // Declare destination path
    char buffer[MAX_BUFFER_LENGTH];      // Declare buffer

    if ((dir = opendir(src_dir)) == NULL)
    {
        printf("ERROR AT PROCESS_FILE");
        perror("ERROR OPENING SOURCE DIRECTORY");
        return NULL;
    }

    int count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        int newer = 0;
        sprintf(buffer, "%s", entry->d_name); // Store entry name in buffer

        if (strcmp(buffer, ".") == 0 || strcmp(buffer, "..") == 0)
        {
            continue; // SKIP . and .. files
        }

        // If the filename starts with ., check if we have a flag enabled
        if (buffer[0] == '.')
        {
            if (!flags->flag_a)
            {
                continue; // Continue if not
            }
        }
        int add_file = 1; // Assume the file will always be added at the start

        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);    // Concatenate a string to form source path, {directory}/{filename}
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name); // Concatenate a string to form destination path, {directory}/{filename}

        struct stat statbuf;
        if (stat(src_path, &statbuf) == -1)
        {
            printf("ERROR HANDLER 1\n");
            perror("ERROR GETTING FILE INFORMATION");
            return NULL;
        }

        // Process i flag
        if (flags->flag_i)
        {
            if (matchRegex(flags, entry->d_name, 1))
            {
                add_file = 0; // Don't add file if it match i pattern
            }
        }

        // Process o flag
        if (flags->flag_o)
        {
            if (!matchRegex(flags, entry->d_name, 0))
            {
                add_file = 0; // Don't add file if it doesn't match o pattern
            }
        }

        // If we still consider adding it
        if (add_file)
        {
            if (searchANDmatch(dest_dir, entry->d_name, statbuf.st_size)) // Check if we already have same file in destination
            {
                if (checkTime(src_path, dest_path) == 3) // If we do, if destination file is newer than source file
                {
                    if (stat(dest_path, &statbuf) == -1)
                    {
                        printf("ERROR HANDLER 2\n");
                        perror("ERROR GETTING FILE INFORMATION");
                        return NULL;
                    }
                    if (flags->flag_v)
                    {
                        printf("Dest : '%s' is newer than Source : '%s'\n", dest_path, src_path); // Verbose output
                    }
                    newer = 1;    // Set newer to 1
                    add_file = 1; // Add file
                    process_v(flags, add_file, entry);
                }
                else if (checkTime(src_path, dest_path) == 2) // If destination file is older than source file
                {
                    if (flags->flag_v)
                    {
                        printf("Source : '%s' is newer than Dest : '%s'\n", src_path, dest_path); // Verbose output
                    }
                    add_file = 1;                      // Add file
                    process_v(flags, add_file, entry); // Verbose output
                }
                else if ((checkTime(src_path, dest_path) == 1))
                {
                    add_file = 0;                      // If they have the same time, they are the same file
                    process_v(flags, add_file, entry); // Verbose output
                }
            }
            else
            {
                add_file = 1;                      // No file matches, add file
                process_v(flags, add_file, entry); // Verbose output
            }
        }

        // Process adding file, these are the files to sync
        if (add_file)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                DIR_COUNT++; // Increment Directory count
            }
            if (S_ISREG(statbuf.st_mode))
            {
                FILE_COUNT++; // Increment File count
            }

            file_info = realloc(file_info, (count + 1) * sizeof(struct FILEINFO)); // Dynamically allocate space for more files to be synced
            if (file_info == NULL)
            {
                printf("ERROR HANDLER 3\n");
                perror("ERROR ALLOCATING MEMORY FOR FILE_INFO");
                return NULL;
            }

            file_info[count].filename = strdup(entry->d_name);     // Set filename
            file_info[count].modification_time = statbuf.st_mtime; // Set modification time
            file_info[count].size = statbuf.st_size;               // Set size
            file_info[count].newer = newer;                        // Set newer
            count++;                                               // Increment count
        }
    }

    closedir(dir);
    *total_count = count; // Assign count to a pointer
    return file_info;
}