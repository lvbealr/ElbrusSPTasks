#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "customWarning.h"
#include "textStruct.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError initializeTextLine(textData *textData, textLine *lineArray) {
    customWarning(textData, TEXT_DATA_BAD_POINTER);
    customWarning(lineArray, LINE_ARRAY_BAD_POINTER);

    return pointTextLine(textData, lineArray);
}

IOError destroyTextLine(textLine *lineArray) {
    customWarning(lineArray, LINE_ARRAY_BAD_POINTER);

    FREE_(lineArray);

    return NO_ERRORS;
}

IOError pointTextLine(textData *textData, textLine *lineArray) {
    customWarning(textData, TEXT_DATA_BAD_POINTER);
    customWarning(lineArray, LINE_ARRAY_BAD_POINTER);

    char *textPointer = textData->text;

    size_t lineIndex = 0;
    lineArray[lineIndex].linePointer = textPointer;

    while (textPointer < textData->text + textData->fileSize) {
        if (*textPointer == '\n') {
            lineArray[lineIndex].lineSize = (size_t)(textPointer - lineArray[lineIndex].linePointer);

            lineIndex++;
            lineArray[lineIndex].linePointer = textPointer + 1;
        }

        textPointer++;
    }

    if (lineIndex < textData->lineCount) {
        lineArray[lineIndex].lineSize = (size_t)(textPointer - lineArray[lineIndex].linePointer);
    }

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError initializeTextData(textData *textData, const char *fileName) {
    customWarning(fileName, FILE_NAME_BAD_POINTER);
    customWarning(textData, TEXT_DATA_BAD_POINTER);

    struct stat fileData;
    stat(fileName, &fileData);

    textData->fileSize = (ssize_t)fileData.st_size;

    textData->text = (char *)calloc((size_t)textData->fileSize, sizeof(char));
    customWarning(textData->text, ALLOCATION_ERROR);

    int fileDescriptor = open(fileName, O_RDONLY);
    customWarning(fileDescriptor != -1, FILE_OPEN_ERROR);

    ssize_t readBytes = read(fileDescriptor, textData->text, (size_t)textData->fileSize);
    customWarning(readBytes == textData->fileSize, FILE_READ_ERROR);

    close(fileDescriptor);

    textData->lineCount = lineCounter(textData);

    textLine *lineArray = (textLine *)calloc(textData->lineCount, sizeof(textLine));
    customWarning(lineArray, ALLOCATION_ERROR);

    textData->lineArray = lineArray;

    initializeTextLine(textData, lineArray);

    return NO_ERRORS;
}

IOError destroyTextData(textData *textData) {
    customWarning(textData, TEXT_DATA_BAD_POINTER);

    textData->fileSize  = 0;
    textData->lineCount = 0;

    FREE_(textData->lineArray);
    FREE_(textData->text);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

size_t lineCounter(textData *textData) {
    customWarning(textData, TEXT_DATA_BAD_POINTER);

    size_t lineCount = {};

    char *textPointer = textData->text;

    while (textPointer < textData->text + textData->fileSize) {
        if (*textPointer == '\n') {
            lineCount++;
        }

        textPointer++;
    }

    if (textData->fileSize > 0 && textData->text[textData->fileSize - 1] != '\n') {
        lineCount++;
    }

    return lineCount;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //