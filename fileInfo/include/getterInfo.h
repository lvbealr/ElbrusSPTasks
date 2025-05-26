#ifndef GETTER_INFO_H_
#define GETTER_INFO_H_

#include <sys/stat.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum getterInfoStatus {
    GETTER_INFO_ERROR     = -1,
    NO_ERRORS             =  0,
    BAD_FILE_NAME         =  1,
    FILE_BAD_POINTER      =  2,
    ALLOCATION_ERROR      =  3,
    FILE_INFO_BAD_POINTER =  4,
    PRINT_INFO_ERROR      =  5,
    DELETE_INFO_ERROR     =  6
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

struct fileInfo {
           char *fileName = {};
    struct stat *info     = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

fileInfo *setFile(const char *fileName);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

getterInfoStatus getFileInfo   (      fileInfo *file);
getterInfoStatus printFileInfo (const fileInfo *file);
getterInfoStatus deleteFileInfo(      fileInfo *file);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // GETTER_INFO_H_