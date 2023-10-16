#include "flags.h"

int main(int argc, char *argv[])
{
    char *ptr = argv[1];
    char word[1024];
    char *word_copy;

    strcpy(word, ptr);

    word_copy = strdup(word);

    printf("%s\n", word_copy);
}