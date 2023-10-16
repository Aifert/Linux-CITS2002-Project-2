#include "mysync.h"

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

    if (getEntryCount(dest_dir) > getEntryCount(src_dir))
    {
        char *temp = src_dir;
        src_dir = dest_dir;
        dest_dir = temp;
    }

    int file_count = 0;
    struct FILEINFO *file_info = process_file(src_dir, dest_dir, flags, &file_count);

    int completion;
    completion = sync_directories(file_info, file_count, src_dir, dest_dir, flags);

    if (completion == 0)
    {
        printf("Synchronization complete.\n");
    }

    return 0;
}
