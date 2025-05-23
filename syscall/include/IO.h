#include <fcntl.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum errorOutputMode {
    BY_ERRNO       = 0,
    BY_SYS_ERRLIST = 1,
    BY_PERROR      = 2
};

const size_t MODE_COUNT = 3;

const int BAD_FILE_DESCRIPTOR = -1;

enum Status {
    FILE_OPEN_ERROR        = -1,
    NO_ERRORS              =  0,
    CONTAINER_BAD_POINTER  =  1,
    FILE_ARRAY_BAD_POINTER =  2,
    FILE_LIST_BAD_POINTER  =  3,
    FILE_BAD_POINTER       =  4,
    BAD_FILE_NAME          =  5
};

struct File {
    char *fileName       = {};
    int   flags          = {};

    int   fileDescriptor = -1;
    errorOutputMode mode = {};
};

struct fileContainer {
    File **files = {};

    size_t size     = {};
    size_t capacity = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

fileContainer *initializeFileContainer(char **fileArray, size_t capacity);
Status         destroyFileContainer   (fileContainer *container);

Status         handleFileNames(fileContainer *container, char **fileArray);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Status         initializeFile(File *file, char *fileName, int flags, errorOutputMode mode);
Status         destroyFile   (File *file);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Status         openFile (File *file);
Status         closeFile(File *file);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //