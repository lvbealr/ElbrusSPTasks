#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "customWarning.h"
#include "sort.h"
#include "textStruct.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

int forwardStrcmp(const void *firstElem, const void *secondElem) {
    customWarning(firstElem, 1);
    customWarning(secondElem, 1);

    const char *firstString =  ((const textLine *)firstElem)->linePointer;
    const char *secondString = ((const textLine *)secondElem)->linePointer;

    size_t firstLen  = ((const textLine *)firstElem)->lineSize;
    size_t secondLen = ((const textLine *)secondElem)->lineSize;

    size_t firstIndex = 0, secondIndex = 0;

    while (firstIndex < firstLen && secondIndex < secondLen) {
        char firstElement  = tolower(firstString[firstIndex]);
        char secondElement = tolower(secondString[secondIndex]);

        if (!isalpha(firstElement) && firstElement != ' ' && firstElement != '\'') {
            firstIndex++;
            continue;
        }
        if (!isalpha(secondElement) && secondElement != ' ' && secondElement != '\'') {
            secondIndex++;
            continue;
        }

        if (firstElement != secondElement) {
            return firstElement - secondElement;
        }

        firstIndex++;
        secondIndex++;
    }

    if (firstIndex >= firstLen && secondIndex >= secondLen) {
        return 0;
    }

    return (firstIndex >= firstLen) ? -1 : 1;
}

int backwardStrcmp(const void *firstElem, const void *secondElem) {
    customWarning(firstElem , 1);
    customWarning(secondElem, 1);

    const char *firstString  = ((const textLine *)firstElem)->linePointer;
    const char *secondString = ((const textLine *)secondElem)->linePointer;

    const size_t firstLen  = ((const textLine *)firstElem)->lineSize;
    const size_t secondLen = ((const textLine *)secondElem)->lineSize;

    size_t firstIndex   = firstLen - 1, secondIndex   = secondLen - 1;
    int    firstElement = 0,            secondElement = 0;

    int asciiDiff = 0;

    while (asciiDiff == 0) {
        firstElement  = firstString[firstIndex];
        secondElement = secondString[secondIndex];

        if (firstElement  == '\n' || firstElement  == EOF) {
            return (secondElement == '\n' || secondElement == EOF) ? 0 : -1;
        }
        
        if (secondElement == '\n' || secondElement == EOF) {
            return 1;
        }
        
        if (isalpha(firstElement) || firstElement  == ' ' || firstElement == '\'') { 
            if (isalpha(secondElement) || secondElement == ' ' || secondElement == '\'') {
                asciiDiff += firstElement - secondElement;
                firstIndex--; secondIndex--;
            }

            else {
                secondIndex--;
            }
        }
        
        else {
            firstIndex--;
        }
    }

    return asciiDiff;
}

int lengthStrcmp(const void *firstElem, const void *secondElem) {
    customWarning(firstElem,  1);
    customWarning(secondElem, 1);

    const textLine *firstLine  = (const textLine *)firstElem;
    const textLine *secondLine = (const textLine *)secondElem;

    const size_t firstLen  = firstLine->lineSize;
    const size_t secondLen = secondLine->lineSize;

    if (firstLen < secondLen) {
        return -1;
    } else if (firstLen > secondLen) {
        return 1;
    } else {
        return forwardStrcmp(firstElem, secondElem);
    }
}

static size_t countWords(const textLine *line) {
    char *lineStart = line->linePointer;
    char *lineEnd   = lineStart + line->lineSize;

    size_t wordCount = 0;

    while (lineStart < lineEnd) {
        while (lineStart < lineEnd && isspace(*lineStart)) {
            lineStart++;
        }

        if (lineStart < lineEnd && !isspace(*lineStart)) {
            wordCount++;

            while (lineStart < lineEnd && !isspace(*lineStart)) {
                lineStart++;
            }
        }
    }

    return wordCount;
}

int wordCountStrcmp(const void *firstElem, const void *secondElem) {
    customWarning(firstElem,  1);
    customWarning(secondElem, 1);

    const textLine *firstLine  = (const textLine *)firstElem;
    const textLine *secondLine = (const textLine *)secondElem;

    size_t firstWordCount  = countWords(firstLine);
    size_t secondWordCount = countWords(secondLine);

    if (firstWordCount < secondWordCount) {
        return -1;
    } else if (firstWordCount > secondWordCount) {
        return 1;
    } else {
        return forwardStrcmp(firstElem, secondElem);
    }
}

int vowelCountStrcmp(const void *firstElem, const void *secondElem) {
    customWarning(firstElem,  1);
    customWarning(secondElem, 1);

    const textLine *firstLine  = (const textLine *)firstElem;
    const textLine *secondLine = (const textLine *)secondElem;

    size_t firstVowelCount  = 0;
    size_t secondVowelCount = 0;

    for (size_t i = 0; i < firstLine->lineSize; i++) {
        char sym = tolower(firstLine->linePointer[i]);
        if (sym == 'a' || sym == 'e' || sym == 'i' || sym == 'o' || sym == 'u') {
            firstVowelCount++;
        }
    }

    for (size_t i = 0; i < secondLine->lineSize; i++) {
        char sym = tolower(secondLine->linePointer[i]);
        if (sym == 'a' || sym == 'e' || sym == 'i' || sym == 'o' || sym == 'u') {
            secondVowelCount++;
        }
    }

    if (firstVowelCount < secondVowelCount) {
        return -1;
    } else if (firstVowelCount > secondVowelCount) {
        return 1;
    } else {
        return forwardStrcmp(firstElem, secondElem);
    }
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError sortTextLines(const textData *textData, compareFunc comparator) {
    customWarning(textData,            TEXT_DATA_BAD_POINTER);
    customWarning(textData->lineArray, LINE_ARRAY_BAD_POINTER);
    customWarning(comparator,          COMPARATOR_BAD_POINTER);

    qsort(textData->lineArray, textData->lineCount, sizeof(textLine), comparator);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //