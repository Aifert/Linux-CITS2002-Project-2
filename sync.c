#include "sync.h"

int exists(const char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

// Function to recursively synchronize directories
int sync_directories(struct FILEINFO *file_info, int total_count, const char *src, const char *dest, FLAG *flags)
{
    char src_path[256];
    char dest_path[256];

    struct FILEINFO *current_file = file_info;
    struct utimbuf times;

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

        if (current_file->newer)
        {
            snprintf(src_path, sizeof(src_path), "%s/%s", dest, current_file->filename);
            snprintf(dest_path, sizeof(dest_path), "%s/%s", src, current_file->filename);
        }

        struct stat statbuf;
        if (stat(src_path, &statbuf) == -1)
        {
            printf("ERROR HANDLER 2\n");
            perror("ERROR GETTING FILE INFORMATION");
            return 1;
        }
        if (S_ISDIR(statbuf.st_mode))
        {
            if (!flags->flag_n)
            {
                if (!exists(dest_path) && mkdir(dest_path, 0777) != 0)
                {
                    perror("ERROR CREATING DIRECTORY");
                    return 1;
                }
            }
            if (flags->flag_r)
            {
                if (flags->flag_v)
                {
                    printf("==> Recursively scanning subdirectory '%s'\n", src_path);
                }
                int temp_file_count = 0;
                struct FILEINFO *temp_file = process_file(src_path, dest_path, flags, &temp_file_count);
                sync_directories(temp_file, temp_file_count, src_path, dest_path, flags);
            }
            if (flags->flag_p)
            {
                times.modtime = current_file->modification_time; // Use the provided modification time
                if (utime(dest_path, &times) == -1)
                {
                    perror("ERROR SETTING MODIFICATION TIME");
                    return 1;
                }
            }
        }
        else
        {
            if (flags->flag_n)
            {
                process_n(src, dest, file_info, total_count);
                return 0;
            }
            else
            {
                int src_file = open(src_path, O_RDONLY);
                if (src_file == -1)
                {
                    perror("ERROR OPENING SOURCE FILE");
                    return 1;
                }

                int dest_file = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (dest_file == -1)
                {
                    perror("ERROR OPENING DESTINATION FILE");
                    return 1;
                }

                char buffer[1024];
                ssize_t bytes_read;

                while ((bytes_read = read(src_file, buffer, sizeof(buffer))) > 0)
                {
                    ssize_t bytes_written = write(dest_file, buffer, bytes_read);
                    if (bytes_written == -1)
                    {
                        perror("Error writing to destination file");
                        break;
                    }
                }
                close(src_file);
                close(dest_file);

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
    if (flags->flag_v)
    {
        process_n(src, dest, file_info, total_count);
    }
    return 0;
}