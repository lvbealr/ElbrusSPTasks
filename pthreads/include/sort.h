#ifndef SORT_H_
#define SORT_H_

#include "sortContext.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus sortArray   (sortContext *context);
void      *sortSubarray(const void *data);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus mergeSubarrays(sortContext *context);
sortStatus merge         (int *array, const size_t left, const size_t mid, const size_t right);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // SORT_H_