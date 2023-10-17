#include "sync.h"

/**
 * Check if a sub directory already exist
 *
 * @param path A pointer to the name of subdirectory
 *
 * @return int
 */
int exists(const char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

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
int sync_directories(struct FILEINFO *file_info, int total_count, const char *src, const char *dest, FLAG *flags)
{
    char src_path[MAX_FILENAME_LENGTH];
    char dest_path[MAX_FILENAME_LENGTH];

    struct FILEINFO *current_file = file_info;

    // Files in source and destination is all same
    if (total_count == 0)
    {
        if (flags->flag_v)
        {
            printf("%i files to be synced.\n", total_count);
            printf("Exiting with success.\n");
        }
        return 1;
    }

    for (int i = 0; i < total_count; i++)
    {
        // Construct source and destination paths
        snprintf(src_path, sizeof(src_path), "%s/%s", src, current_file->filename);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, current_file->filename);

        // Check if the file in destination is newer than source
        if (current_file->newer)
        {
            snprintf(src_path, sizeof(src_path), "%s/%s", dest, current_file->filename);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", src, current_file->filename);
        }

        struct stat statbuf;
        if (stat(src_path, &statbuf) == -1)
        {
            printf("src path %s\n", src_path);
            printf("ERROR HANDLER 4\n");
            perror("ERROR GETTING FILE INFORMATION");
            return 1;
        }

        // Check if current name in file is a directory
        if (S_ISDIR(statbuf.st_mode))
        {
            // Check for n flag
            if (!flags->flag_n)
            {
                // Creating a directory if it doesn't exist already
                if (!exists(dest_path) && mkdir(dest_path, 0777) != 0)
                {
                    perror("ERROR CREATING DIRECTORY");
                    return 1;
                }
            }
            // Check if r flag is enabled
            if (flags->flag_r)
            {
                if (flags->flag_v)
                {
                    printf("==> Recursively scanning subdirectory '%s'\n", src_path); // Verbose output
                }
                // Create a temporary file count and FILEINFO named temp_file to recursively sync directory
                int temp_file_count = 0;
                struct FILEINFO *temp_file = process_file(src_path, dest_path, flags, &temp_file_count);
                sync_directories(temp_file, temp_file_count, src_path, dest_path, flags);
            }
        }
        // if not directory must be a regular file
        else
        {
            if (flags->flag_n)
            {
                process_n(src, dest, file_info, total_count);
                return 0;
            }
            else
            {
                int src_file = open(src_path, O_RDONLY); // Open source path for reading
                if (src_file == -1)
                {
                    printf("ERROR HANDLER 5\n");
                    perror("ERROR OPENING SOURCE FILE");
                    return 1;
                }

                int dest_file = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666); // Open destination path for writing and creating, setting permissions to 0666
                if (dest_file == -1)
                {
                    printf("ERROR HANDLER 6\n");
                    perror("ERROR OPENING DESTINATION FILE");
                    return 1;
                }

                char buffer[MAX_BUFFER_LENGTH];
                ssize_t bytes_read;

                // Begin syncing files
                while ((bytes_read = read(src_file, buffer, sizeof(buffer))) > 0)
                {
                    ssize_t bytes_written = write(dest_file, buffer, bytes_read); // Writing data to destination file
                    if (bytes_written == -1)
                    {
                        printf("ERROR HANDLER 7\n");
                        perror("Error writing to destination file");
                        break;
                    }
                }
                close(src_file);
                close(dest_file);

                // Set modification time for file if p flag is enabled
                if (flags->flag_p)
                {
                    struct utimbuf times;
                    times.modtime = current_file->modification_time;
                    if (utime(dest_path, &times) == -1)
                    {
                        perror("ERROR SETTING MODIFICATION TIME");
                        return 1;
                    }
                    if (chmod(dest_path, statbuf.st_mode) == -1)
                    {
                        perror("ERRIR SETTING FILE PERMISSIONS");
                        return 1;
                    }
                }
            }
        }
        current_file++;
    }
    // Print verbose output
    if (flags->flag_v)
    {
        process_n(src, dest, file_info, total_count);
    }
    return 0;
}