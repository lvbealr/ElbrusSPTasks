#ifndef SORT_H_
#define SORT_H_

#include "textStruct.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

typedef int (*compareFunc)(const void *firstElem, const void *secondElem);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

int forwardStrcmp   (const void *firstElem, const void *secondElem);
int backwardStrcmp  (const void *firstElem, const void *secondElem);
int lengthStrcmp    (const void *firstElem, const void *secondElem);
int wordCountStrcmp (const void *firstElem, const void *secondElem);
int vowelCountStrcmp(const void *firstElem, const void *secondElem);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

IOError sortTextLines(const textData *textData, compareFunc comparator);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // SORT_H_