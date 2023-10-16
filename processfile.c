#include "processfile.h"

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

int searchANDmatch(const char *dest_dir, const char *filename, long long size)
{
    DIR *dir;
    struct dirent *entry;
    // struct stat statbuf;
    char file_path[256];

    if ((dir = opendir(dest_dir)) == NULL)
    {
        perror("ERROR OPENING DESTINATION DIRECTORY");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        snprintf(file_path, sizeof(file_path), "%s/%s", dest_dir, filename);

        if (match(file_path, size))
        {
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

int checkTime(const char *src_path, const char *dest_path)
{
    struct stat src_statbuf;
    struct stat dest_statbuf;

    if (stat(src_path, &src_statbuf) == -1)
    {
        perror("ERROR GETTING SOURCE FILE INFORMATION");
        return 0;
    }

    if (stat(dest_path, &dest_statbuf) == -1)
    {
        perror("ERROR GETTING DEST FILE INFORMATION");
        return 0;
    }

    if (dest_statbuf.st_mtime > src_statbuf.st_mtime)
    {
        return 3;
    }
    else if (dest_statbuf.st_mtime < src_statbuf.st_mtime)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

int getEntryCount(const char *dir, FLAG *flags)
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

struct FILEINFO *process_file(const char *src_dir, const char *dest_dir, FLAG *flags, int *total_count)
{
    DIR *dir;
    struct dirent *entry;
    struct FILEINFO *file_info = NULL;

    char src_path[MAX_FILENAME_LENGTH];
    char dest_path[MAX_FILENAME_LENGTH];
    char buffer[MAX_BUFFER_LENGTH];

    if ((dir = opendir(src_dir)) == NULL)
    {
        perror("ERROR OPENING SOURCE DIRECTORY");
        return NULL;
    }

    int count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        sprintf(buffer, "%s", entry->d_name);

        if (strcmp(buffer, ".") == 0 || strcmp(buffer, "..") == 0)
        {
            continue;
        }
        if (buffer[0] == '.')
        {
            if (!process_a(flags))
            {
                continue;
            }
        }
        int add_file = 1; // ASSUME THE FILE WILL BE ADDED UNLESS A FLAG PREVENTS IT

        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

        struct stat statbuf;
        if (stat(src_path, &statbuf) == -1)
        {
            printf("ERROR HANDLER 1\n");
            perror("ERROR GETTING FILE INFORMATION");
            return NULL;
        }

        if (flags->flag_i)
        {
            if (matchRegex(flags->pattern_i, entry->d_name))
            {
                add_file = 0;
            }
        }

        if (flags->flag_o)
        {
            if (!matchRegex(flags->pattern_o, entry->d_name))
            {
                add_file = 0;
            }
        }

        if (add_file)
        {
            if (searchANDmatch(dest_dir, entry->d_name, statbuf.st_size))
            {
                if (checkTime(src_path, dest_path) == 3)
                {
                    if (stat(dest_path, &statbuf) == -1)
                    {
                        perror("ERROR GETTING FILE INFORMATION");
                        printf("Dest : '%s' is newer than Source : '%s'\n", dest_path, src_path);
                        add_file = 1;
                        process_v(flags, add_file, entry);
                    }
                }
                else if (checkTime(src_path, dest_path) == 2)
                {
                    printf("Source : '%s' is newer than Dest : '%s'\n", src_path, dest_path);
                    add_file = 1;
                    process_v(flags, add_file, entry);
                }
                else if ((checkTime(src_path, dest_path) == 1))
                {
                    add_file = 0;
                    process_v(flags, add_file, entry);
                }
            }
            else
            {
                add_file = 1;
                process_v(flags, add_file, entry);
            }
        }

        if (add_file)
        {
            if (S_ISDIR(statbuf.st_mode))
            {
                DIR_COUNT++;
            }
            if (S_ISREG(statbuf.st_mode))
            {
                FILE_COUNT++;
            }
            file_info = realloc(file_info, (count + 1) * sizeof(struct FILEINFO));
            if (file_info == NULL)
            {
                perror("ERROR ALLOCATING MEMORY FOR FILE_INFO");
                return NULL;
            }

            file_info[count].filename = strdup(entry->d_name);
            file_info[count].modification_time = statbuf.st_mtime;
            file_info[count].size = statbuf.st_size;
            count++;
        }
    }

    closedir(dir);
    *total_count = count;
    return file_info;
}