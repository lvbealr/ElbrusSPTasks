#include "getterInfo.h"

#include <stdio.h>
#include "customWarning.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        customPrint(red, bold, bgDefault, "Usage: %s <file_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    fileInfo *file = setFile(argv[1]);

    getterInfoStatus status = getFileInfo(file);
    customWarning(status == NO_ERRORS, GETTER_INFO_ERROR);

    status = printFileInfo(file);
    customWarning(status == NO_ERRORS, PRINT_INFO_ERROR);

    status = deleteFileInfo(file);
    customWarning(status == NO_ERRORS, DELETE_INFO_ERROR);

    return EXIT_SUCCESS;
}