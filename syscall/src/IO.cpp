#include <cstdio>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "IO.h"
#include "customWarning.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static const uint32_t FILE_MODE[] = {O_RDONLY, O_WRONLY, O_RDWR, 
                                     O_APPEND, O_CREAT,  O_DSYNC, 
                                     O_EXCL,   O_NOCTTY, O_NONBLOCK, 
                                     O_RSYNC, O_SYNC,  O_TRUNC};

static const size_t MAX_FILE_MODE = sizeof(FILE_MODE) / sizeof(FILE_MODE[0]);

static const unsigned int SEED = 42;

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

fileContainer *initializeFileContainer(char **fileArray, size_t capacity) {
    customWarning(fileArray, NULL);

    fileContainer *container = (fileContainer *)calloc(1, sizeof(fileContainer));
    customWarning(container, NULL);

    container->size     = 0;
    container->capacity = capacity > 0 ? capacity : 1;

    container->files = (File **)calloc(container->capacity, sizeof(File *));
    if (!container->files) {
        FREE_(container);
        customWarning(container->files, NULL);
    }

    for (size_t i = 0; i < container->capacity; i++) {
        container->files[i] = (File *)calloc(1, sizeof(File));
        
        if (!container->files[i]) {
            for (size_t j = 0; j < i; j++) {
                FREE_(container->files[j]);
            }

            FREE_(container->files);
            FREE_(container);

            customWarning(container->files[i], NULL);
        }
    }

    handleFileNames(container, fileArray);

    return container;
}

Status destroyFileContainer(fileContainer *container) {
    customWarning(container, CONTAINER_BAD_POINTER);

    if (container->files) {
        for (size_t i = 0; i < container->size; i++) {
            if (container->files[i]) {
                destroyFile(container->files[i]);
            }
        }
    }

    FREE_(container->files);

    container->size     = 0;
    container->capacity = 0;

    FREE_(container);

    return NO_ERRORS;
}

Status handleFileNames(fileContainer *container, char **fileArray) {
    srand(SEED);

    customWarning(container,        CONTAINER_BAD_POINTER);
    customWarning(container->files, FILE_LIST_BAD_POINTER);
    customWarning(fileArray,        FILE_ARRAY_BAD_POINTER);

    size_t capacity = container->capacity;

    for (size_t i = 0; i < capacity; i++) {
        size_t   flagsCount = rand() % MAX_FILE_MODE;
        uint32_t fileFlags  = {};

        for (size_t j = 0; j < flagsCount; j++) {
            size_t flagIndex = rand() % MAX_FILE_MODE;
            fileFlags |= FILE_MODE[flagIndex];
        }
        
        errorOutputMode mode = (errorOutputMode)(rand() % MODE_COUNT);

        initializeFile(container->files[i], fileArray[i], fileFlags, mode);

        container->size++;
    }

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Status initializeFile(File *file, char *fileName, int flags, errorOutputMode mode) {
    customWarning(file,     FILE_BAD_POINTER);
    customWarning(fileName, BAD_FILE_NAME);

    file->fileName = fileName;
    file->flags    = flags;
    file->mode     = mode;

    return NO_ERRORS;
}

Status destroyFile(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    if (file->fileDescriptor != BAD_FILE_DESCRIPTOR) {
        closeFile(file);
    }

    file->fileName = NULL;
    file->flags    = {};
    file->mode     = {};

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Status openFile(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    file->fileDescriptor = open(file->fileName, file->flags);

    if (file->fileDescriptor == FILE_OPEN_ERROR) {
        switch (file->mode) {
            case BY_ERRNO: {
                customPrint(red, bold, bgDefault, "[ ERROR #%d ]: (file: %s) ", errno, file->fileName);
                fprintf(stderr, "%s\n", strerror(errno));
                return FILE_OPEN_ERROR;
            }

            case BY_SYS_ERRLIST: {
                if (errno >= 0 && errno < sys_nerr) {
                    customPrint(red, bold, bgDefault, "[ ERROR #%d ]: (file: %s) ", errno, file->fileName);
                    fprintf(stderr, "%s\n", sys_errlist[errno]);
                    return FILE_OPEN_ERROR;
                }
            }

            case BY_PERROR: {
                customPrint(red, bold, bgDefault, "[ ERROR #%d ]: (file: %s) ", errno, file->fileName);
                perror(strerror(errno));
                return FILE_OPEN_ERROR;
            }

            default: {
                return FILE_OPEN_ERROR;
            }
        }
    }

    return NO_ERRORS;
}

Status closeFile(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    if (file->fileDescriptor != BAD_FILE_DESCRIPTOR) {
        close(file->fileDescriptor);
    }

    file->fileDescriptor = BAD_FILE_DESCRIPTOR;

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //