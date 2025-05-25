#ifndef IO_H_
#define IO_H_

#include <stdlib.h>
#include <sys/stat.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum IOError {
    FILE_READ_ERROR          = -1,
    NO_ERRORS                =  0,
    ALLOCATION_ERROR         =  1,
    FILE_BAD_POINTER         =  2,
    FILE_PATH_BAD_POINTER    =  3,
    FILE_GET_INFO_ERROR      =  4,
    SETTING_FILE_ERROR       =  5,
    FILE_CLOSE_ERROR         =  6,
    FILE_OPEN_ERROR          =  7,
    FILE_WRITE_ERROR         =  8,
    BAD_SUFFIX               =  9,
    BAD_FILE_NAME            = 10,
    BAD_PID                  = 11,
    CHILD_PROCESS_ERROR      = 12,
    FILE_COPY_ERROR          = 13,
    FILE_CONTENT_BAD_POINTER = 14
};

struct File {
    char *path = {};

    int    fileDescriptor = -1;
    struct stat *fileInfo = {};

    char *fileContent = {};

    File *fileCopy = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

File   *setFile  (const char *path);
IOError unsetFile(File       *file);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError openFile   (File *file);
IOError getFileInfo(File *file);
IOError copyFile   (File *file, const char *suffix);
IOError closeFile  (File *file);

IOError showContent(File *file);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // IO_H_