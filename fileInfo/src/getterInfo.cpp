#include <stdlib.h>
#include <string.h>

#include "customWarning.h"
#include "getterInfo.h"

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

fileInfo *setFile(const char *fileName) {
    customWarning(fileName, NULL);

    fileInfo *file = (fileInfo *)calloc(1, sizeof(fileInfo));
    customWarning(file, NULL);

    size_t fileNameLength = strlen(fileName);

    file->fileName = (char *)calloc(fileNameLength + 1, sizeof(char));
    customWarning(file->fileName, NULL);

    strncpy(file->fileName, fileName, fileNameLength);
    file->fileName[fileNameLength] = '\0';

    file->info = NULL;

    return file;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

getterInfoStatus getFileInfo(fileInfo *file) {
    customWarning(file,           FILE_BAD_POINTER);
    customWarning(file->fileName, BAD_FILE_NAME);

    file->info = (struct stat *)calloc(1, sizeof(struct stat));
    customWarning(file->info, ALLOCATION_ERROR);

    getterInfoStatus status = (getterInfoStatus)stat(file->fileName, file->info);
    customWarning(status == NO_ERRORS, GETTER_INFO_ERROR);

    return NO_ERRORS;
};

getterInfoStatus printFileInfo(const fileInfo *file) {
    customWarning(file,           FILE_BAD_POINTER);
    customWarning(file->fileName, BAD_FILE_NAME);
    customWarning(file->info,     FILE_INFO_BAD_POINTER);

    customPrint(purple, bold, bgDefault, "File: %s\n", file->fileName);

    // --------------------------------------------------- stat ------------------------------------------------- //
    customPrint(white, bold, bgDefault, "st_dev (device): %lu\n",                    file->info->st_dev);
    customPrint(white, bold, bgDefault, "st_ino (inode): %lu\n",                     file->info->st_ino);
    customPrint(white, bold, bgDefault, "st_mode (mode): %o\n",                      file->info->st_mode);
    customPrint(white, bold, bgDefault, "st_nlink (number of links): %lu\n",         file->info->st_nlink);
    customPrint(white, bold, bgDefault, "st_uid (user ID): %u\n",                    file->info->st_uid);
    customPrint(white, bold, bgDefault, "st_gid (group ID): %u\n",                   file->info->st_gid);
    customPrint(white, bold, bgDefault, "st_rdev (device type): %lu\n",              file->info->st_rdev);
    customPrint(white, bold, bgDefault, "st_size (size in bytes): %ld\n",            file->info->st_size);
    customPrint(white, bold, bgDefault, "st_blksize (block size): %ld\n",            file->info->st_blksize);
    customPrint(white, bold, bgDefault, "st_blocks (number of blocks): %ld\n",       file->info->st_blocks);
    customPrint(white, bold, bgDefault, "st_atime (last access time): %ld\n",        file->info->st_atime);
    customPrint(white, bold, bgDefault, "st_mtime (last modification time): %ld\n",  file->info->st_mtime);
    customPrint(white, bold, bgDefault, "st_ctime (last status change time): %ld\n", file->info->st_ctime);
    // ---------------------------------------------------------------------------------------------------------- //

    return NO_ERRORS;
}

getterInfoStatus deleteFileInfo(fileInfo *file) {
    customWarning(file, FILE_BAD_POINTER);

    if (file->fileName) {
        FREE_(file->fileName);
    }

    if (file->info) {
        FREE_(file->info);
    }

    FREE_(file);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //