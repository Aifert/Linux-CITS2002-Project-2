#include "mysync.h"

int DIR_COUNT = 0;
int FILE_COUNT = 0;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s src_dir dest_dir\n", argv[0]);
        return 1;
    }

    FLAG *flags = checkflag(argc, argv);

    char *src_dir = argv[optind];
    optind++;
    char *dest_dir = argv[optind];

    if (getEntryCount(dest_dir, flags) > getEntryCount(src_dir, flags))
    {
        char *temp = src_dir;
        src_dir = dest_dir;
        dest_dir = temp;
    }

    if (flags->flag_v)
    {
        printf("Initializing Synchronization......\n");
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

    return 0;
}
