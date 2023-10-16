#include "processfile.h"

int match(const char *file_path, const char *filename, long long size)
{
    struct stat buffer;

    if (stat(file_path, &buffer) != 0)
    {
        return 0; // File does not exist
    }

    // Check if the size and modification time match
    if (size == buffer.st_size && filename[0] != '.')
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

        if (match(file_path, filename, size))
        {
            return 1;
        }
    }

    closedir(dir);
    return 0;
}

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

struct FILEINFO *process_file(const char *src_dir, const char *dest_dir, FLAG *flags, int *file_count)
{
    DIR *dir;
    struct dirent *entry;
    struct FILEINFO *file_info = NULL;

    char src_path[512];
    char dest_path[512];
    char buffer[1024];

    if ((dir = opendir(src_dir)) == NULL)
    {
        perror("ERROR OPENING SOURCE DIRECTORY");
        return NULL;
    }

    int count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        sprintf(buffer, "%s", entry->d_name);

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
                add_file = 0;
                process_v(flags, 0, dest_path, entry);
            }
            else
            {
                process_v(flags, add_file, dest_path, entry);
            }
        }

        if (add_file)
        {
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
    *file_count = count;
    return file_info;
}