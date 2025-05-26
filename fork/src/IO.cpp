#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "IO.h"
#include "customWarning.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

File *setFile(const char *path) {
    customWarning(path, NULL);

    File *file = (File *)calloc(1, sizeof(File));
    customWarning(file, NULL);

    size_t pathLength = strlen(path);

    file->path = (char *)calloc(pathLength + 1, sizeof(char));
    customWarning(file->path, NULL);

    strncpy(file->path, path, pathLength);
    file->path[pathLength] = '\0';

    getFileInfo(file);

    return file;
}

IOError unsetFile(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    if (file->path) {
        FREE_(file->path);
    }

    if (file->fileDescriptor != -1) {
        closeFile(file);
    }

    if (file->fileInfo) {
        FREE_(file->fileInfo);
    }

    if (file->fileContent) {
        FREE_(file->fileContent);
    }

    if (file->fileCopy) {
        unsetFile(file->fileCopy);
    }

    FREE_(file);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError openFile(File *file) {
    customWarning(file,       FILE_BAD_POINTER);
    customWarning(file->path, FILE_PATH_BAD_POINTER);

    file->fileDescriptor = open(file->path, O_RDWR | O_CREAT, 0644);
    customWarning(file->fileDescriptor != -1, FILE_OPEN_ERROR);

    return NO_ERRORS;
}

IOError getFileInfo(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    file->fileInfo = (struct stat *)calloc(1, sizeof(struct stat));
    customWarning(file->fileInfo, ALLOCATION_ERROR);

    if (stat(file->path, file->fileInfo) == -1) {
        FREE_(file->fileInfo);
        return FILE_GET_INFO_ERROR;
    }

    return NO_ERRORS;
}

IOError copyFile(File *file, const char *suffix) {
    customWarning(file,   FILE_BAD_POINTER);
    customWarning(suffix, BAD_SUFFIX);

    if (file->fileCopy) {
        unsetFile(file->fileCopy);
    }

    char *copyPath = (char *)calloc(strlen(file->path) + strlen(suffix) + 1, sizeof(char));
    customWarning(copyPath, ALLOCATION_ERROR);

    snprintf(copyPath, strlen(file->path) + strlen(suffix) + 1, "%s%s", file->path, suffix);

    file->fileCopy = setFile(copyPath);
    customWarning(file->fileCopy, SETTING_FILE_ERROR);

    file->fileContent = (char *)calloc(file->fileInfo->st_size, sizeof(char));
    customWarning(file->fileContent, ALLOCATION_ERROR);

    lseek(file->fileDescriptor, 0, SEEK_SET);

    ssize_t readSize = read(file->fileDescriptor, file->fileContent, file->fileInfo->st_size);
    customWarning(readSize == file->fileInfo->st_size, FILE_READ_ERROR);

    IOError openStatus = openFile(file->fileCopy);
    customWarning(openStatus == NO_ERRORS, openStatus);

    ssize_t writeSize = write(file->fileCopy->fileDescriptor, file->fileContent, readSize);
    customWarning(writeSize == readSize, FILE_WRITE_ERROR);

    IOError closeStatus = closeFile(file->fileCopy);
    customWarning(closeStatus == NO_ERRORS, FILE_CLOSE_ERROR);

    IOError infoStatus = getFileInfo(file->fileCopy);
    customWarning(infoStatus == NO_ERRORS, FILE_GET_INFO_ERROR);

    file->fileCopy->fileContent = (char *)calloc(readSize + 1, sizeof(char));
    memcpy(file->fileCopy->fileContent, file->fileContent, readSize);
    file->fileCopy->fileContent[readSize] = '\0';

    FREE_(copyPath);
    return NO_ERRORS;
}

IOError closeFile(File *file) {
    customWarning(file, FILE_BAD_POINTER);

    if (file->fileDescriptor != -1) {
        if (close(file->fileDescriptor) == -1) {
            return FILE_CLOSE_ERROR;
        }

        file->fileDescriptor = -1;
    }

    return NO_ERRORS;
}

IOError showContent(File *file) {
    customWarning(file,              FILE_BAD_POINTER);
    customWarning(file->path,        FILE_PATH_BAD_POINTER);
    customWarning(file->fileContent, FILE_CONTENT_BAD_POINTER);

    customPrint(purple, bold, bgDefault, "%s:\n", file->path);
    customPrint(white,  bold, bgDefault, "%s\n",  file->fileContent);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //