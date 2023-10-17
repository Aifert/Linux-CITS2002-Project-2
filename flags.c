#include "flags.h"

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if (dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

FLAG *checkflag(int argc, char *arg_value[])
{
    FLAG *flags = calloc(1, sizeof(FLAG));

    if (flags == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int opt;
    while ((opt = getopt(argc, arg_value, OPTLIST)) != -1)
    {
        switch (opt)
        {
        case 'a':
            flags->flag_a = 1;
            break;
        case 'i':
            flags->flag_i = 1;
            flags->patterns_i = realloc(flags->patterns_i, sizeof(char *) * (flags->pattern_i_count + 1));
            if (flags->patterns_i == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            flags->patterns_i[flags->pattern_i_count] = strdup(optarg);
            flags->pattern_i_count++; // Store the argument of -i option
            break;
        case 'n':
            flags->flag_n = 1;
            break;
        case 'o':
            flags->flag_o = 1;
            flags->patterns_o = realloc(flags->patterns_o, sizeof(char *) * (flags->pattern_o_count + 1));
            if (flags->patterns_o == NULL)
            {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            flags->patterns_o[flags->pattern_o_count] = strdup(optarg);
            flags->pattern_o_count++; // Store the argument of -i option
            break;
        case 'r':
            flags->flag_r = 1;
            break;
        case 'v':
            flags->flag_v = 1;
            break;
        case 'p':
            flags->flag_p = 1;
            break;
        }
    }

    return flags;
}

bool process_a(FLAG *flags)
{
    if (flags->flag_a == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void process_n(const char *src_dir, const char *dest_dir, struct FILEINFO *file_info, int file_count)
{
    char src_path[256];
    char dest_path[256];
    struct FILEINFO *current_file = file_info;

    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < file_count; i++)
    {
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, current_file->filename);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, current_file->filename);
        printf("Copy from %s to %s\n", src_path, dest_path);
        printf("Copying %llo bytes......\n\n", current_file->size);
        current_file++;
    }

    printf("--------------------------------------------------------------------------------\n");
}

void process_v(FLAG *flags, int accept, struct dirent *entry)
{
    if (flags->flag_v || flags->flag_n)
    {
        if (accept)
        {
            printf("ACCEPTING file '%s'\n", entry->d_name);
        }
        else
        {
            printf("SKIPPING file '%s'\n", entry->d_name);
        }
    }
}

void printflags(FLAG *flags)
{
    printf("flag_a: %d\n", flags->flag_a);
    printf("flag_n: %d\n", flags->flag_n);
    printf("flag_r: %d\n", flags->flag_r);
    printf("flag_v: %d\n", flags->flag_v);
    printf("flag_p: %d\n", flags->flag_p);
    printf("flag_i: %d\n", flags->flag_i);
    printf("flag_o: %d\n", flags->flag_o);

    for (int i = 0; i < flags->pattern_i_count; i++)
    {
        printf("pattern_i[%d]: %s\n", i, flags->patterns_i[i]);
    }

    for (int i = 0; i < flags->pattern_o_count; i++)
    {
        printf("pattern_o[%d]: %s\n", i, flags->patterns_o[i]);
    }
}
