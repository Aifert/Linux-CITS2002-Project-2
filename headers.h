#ifndef HEADERS_H
#define HEADERS_H

#define MAX_FILENAME_LENGTH 512 // Declare MAX FILENAME LENGTH
#define MAX_BUFFER_LENGTH 1024  // Declare MAX BUFFER LENGTH

extern int DIR_COUNT;  // Global directory count variable
extern int FILE_COUNT; // Global file count variable

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <getopt.h>
#include <utime.h>
#include <regex.h>

#endif // HEADERS_H
