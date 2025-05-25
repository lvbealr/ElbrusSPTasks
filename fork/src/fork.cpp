#include "fork.h"
#include "IO.h"
#include "customWarning.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError processFork(const char *fileName) {
    customWarning(fileName, BAD_FILE_NAME);

    File *parentFile = setFile(fileName);
    customWarning(parentFile, SETTING_FILE_ERROR);

    IOError status = NO_ERRORS;

    status = openFile(parentFile);
    customWarning(status == NO_ERRORS, FILE_OPEN_ERROR);

    status = getFileInfo(parentFile);
    customWarning(status == NO_ERRORS, FILE_GET_INFO_ERROR);

    pid_t pid = fork();

    if (pid == -1) {
        unsetFile(parentFile);
        return BAD_PID;
    }

    if (pid == 0) {
        const char *suffix = ".child";
        status = copyFile(parentFile, suffix);

        if (status != NO_ERRORS) {
            customPrint(red, bold, bgDefault, "Child process error! Exit...\n");
            exit(status);
        }

        showContent(parentFile->fileCopy);

        unsetFile(parentFile);
        exit(NO_ERRORS);
    } else {
        int statusCode = {};

        customWarning(waitpid(pid, &statusCode, 0) != -1, CHILD_PROCESS_ERROR);

        customWarning(WIFEXITED  (statusCode) != 0,         CHILD_PROCESS_ERROR);
        customWarning(WEXITSTATUS(statusCode) == NO_ERRORS, CHILD_PROCESS_ERROR);
        
        const char *suffix = ".parent";

        status = copyFile(parentFile, suffix);
        customWarning(status == NO_ERRORS, FILE_COPY_ERROR);

        showContent(parentFile->fileCopy);

        unsetFile(parentFile);

        return NO_ERRORS;
    }

}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //