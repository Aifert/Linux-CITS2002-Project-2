#include "mysync.h"

int DIR_COUNT = 0;
int FILE_COUNT = 0;

struct DirectoryInfo *directories = NULL;

void initializeDirectories(int num_dir)
{
    directories = (struct DirectoryInfo *)malloc(num_dir * sizeof(struct DirectoryInfo));

    if (directories == NULL)
    {
        printf("MEMORY ALLOCATION FAILED. \n");
        exit(EXIT_FAILURE);
    }
}

void cleanDirectories()
{
    free(directories);
}

void fillDir(char *argv[], FLAG *flags, int num_dir)
{
    for (int i = 0; i < num_dir; i++)
    {
        directories[i].dir_name = argv[optind + i];
        directories[i].entry_count = getEntryCount(argv[optind + i], flags);
    }
}

int maxEntry(int num_dir)
{
    int max_entry = 0;
    int max_index = 0;

    for (int i = 0; i < num_dir; i++)
    {
        if (directories[i].entry_count >= max_entry)
        {
            max_entry = directories[i].entry_count;
            max_index = i;
        }
    }

    return max_index;
}

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

    initializeDirectories(num_dir);

    fillDir(argv, flags, num_dir);
    if (flags->flag_v)
    {
        printf("Created space to sync %i directories. \n", num_dir);
    }

    int max_index = maxEntry(num_dir);

    while (num_dir != 0)
    {
        if (x == max_index)
        {
            x++;
            num_dir--;
            continue;
        }

        char *src_dir = directories[max_index].dir_name;
        char *dest_dir = directories[x].dir_name;

        if (flags->flag_v)
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

        if (completion == 0)
        {
            if (flags->flag_v)
            {
                printf("%i files, %i directories.\n", FILE_COUNT, DIR_COUNT);
            }
            printf("Synchronization complete.\n");
        }
        x++;
        num_dir--;
        DIR_COUNT = 0;
        FILE_COUNT = 0;
    }

    cleanDirectories();
    return 0;
}
