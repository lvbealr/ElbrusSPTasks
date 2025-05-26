#ifndef TEXT_STRUCT_H_
#define TEXT_STRUCT_H_

#include <stdlib.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while(0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum IOError {
    NO_ERRORS              = 0,
    TEXT_DATA_BAD_POINTER  = 1,
    LINE_ARRAY_BAD_POINTER = 2,
    FILE_NAME_BAD_POINTER  = 3,
    ALLOCATION_ERROR       = 4,
    FILE_OPEN_ERROR        = 5,
    FILE_READ_ERROR        = 6,
    COMPARATOR_BAD_POINTER = 7,
};

struct textLine {
    char  *linePointer = {};
    size_t lineSize    = {};
};

struct textData {
    ssize_t fileSize = {};
    
    textLine *lineArray = {};
    size_t    lineCount  = {};

    char *text = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError initializeTextLine(textData *textData, textLine *lineArray);
IOError destroyTextLine   (textLine *lineArray);
IOError pointTextLine     (const textData *textData, textLine *lineArray);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError initializeTextData(textData *textData, const char *fileName);
IOError destroyTextData   (textData *textData);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

size_t  lineCounter       (const textData *textData);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // TEXT_STRUCT_H_