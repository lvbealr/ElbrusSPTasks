#include <stdlib.h>
#include <unistd.h>

#include "customWarning.h"
#include "sort.h"
#include "sortContext.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static int comparator(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus sortArray(sortContext *context) {
    customWarning(context,          CONTEXT_BAD_POINTER);
    customWarning(context->data,    ARRAY_BAD_POINTER);
    customWarning(context->threads, THREADS_ARRAY_BAD_POINTER);

    size_t maxThreadsCount = sysconf(_SC_NPROCESSORS_ONLN);

    if (context->threads->threadsCount > maxThreadsCount) {
        context->threads->threadsCount = maxThreadsCount;
    }

    size_t subarraySize     = context->data->size / context->threads->threadsCount;
    size_t lastSubarraySize = context->data->size % context->threads->threadsCount;

    context->threads->chunks = (Chunk **)calloc(context->threads->threadsCount, sizeof(Chunk *));
    customWarning(context->threads->chunks, ALLOCATION_ERROR);

    for (size_t idx = 0; idx < context->threads->threadsCount; idx++) {
        Chunk *arrayChunk = (Chunk *)calloc(1, sizeof(Chunk));
        customWarning(arrayChunk, ALLOCATION_ERROR);

        arrayChunk->array = context->data->array;

        arrayChunk->startIndex =  idx      * subarraySize;
        arrayChunk->endIndex   = (idx + 1) * subarraySize;

        if (idx == context->threads->threadsCount - 1) {
            arrayChunk->endIndex += lastSubarraySize;
        }

        int status = pthread_create(&context->threads->threads[idx], NULL, (void *(*)(void *))sortSubarray, arrayChunk);

        if (status != 0) {
            FREE_(arrayChunk);
            FREE_(context->threads->chunks);

            customWarning(status == 0, THREAD_CREATION_ERROR);
        }

        context->threads->chunks[idx] = arrayChunk;
    }

    for (size_t idx = 0; idx < context->threads->threadsCount; idx++) {
        pthread_join(context->threads->threads[idx], NULL);
    }

    sortStatus mergeStatus = mergeSubarrays(context);
    customWarning(mergeStatus == NO_ERRORS, MERGE_ERROR);

    for (size_t idx = 0; idx < context->threads->threadsCount; idx++) {
        FREE_(context->threads->chunks[idx]);
    }

    FREE_(context->threads->chunks);

    return NO_ERRORS;
}

void *sortSubarray(const void *data) {
    Chunk *arrayChunk = (Chunk *)data;

    qsort(arrayChunk->array    + arrayChunk->startIndex,
         arrayChunk->endIndex - arrayChunk->startIndex, sizeof(int), comparator);

    return NULL;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus mergeSubarrays(sortContext *context) {
    customWarning(context,                  CONTEXT_BAD_POINTER);
    customWarning(context->threads,         THREADS_BAD_POINTER);
    customWarning(context->threads->chunks, CHUNKS_BAD_POINTER);

    size_t threadsCount = context->threads->threadsCount;

    size_t *startIndexes = (size_t *)calloc(threadsCount, sizeof(size_t));
    customWarning(startIndexes, ALLOCATION_ERROR);

    size_t *endIndexes = (size_t *)calloc(threadsCount, sizeof(size_t));
    if (!endIndexes) {
        FREE_(startIndexes);

        customWarning(endIndexes, ALLOCATION_ERROR);
    }

    for (size_t i = 0; i < threadsCount; i++) {
        startIndexes[i] = context->threads->chunks[i]->startIndex;
        endIndexes  [i] = context->threads->chunks[i]->endIndex;
    }

    size_t step = 1;

    while (step < threadsCount) {
        for (size_t i = 0; i < threadsCount; i += step * 2) {
            size_t left  = startIndexes[i];
            size_t mid   = endIndexes  [i];
            size_t right = {};

            if (i + step < threadsCount) {
                right = endIndexes[i + step];
            } else {
                right = context->data->size;
            }

            if (mid < right) {
                merge(context->data->array, left, mid - 1, right - 1);

                if (i + step < threadsCount) {
                    endIndexes[i] = right;
                }
            }
        }

        step *= 2;
    }

    FREE_(startIndexes);
    FREE_(endIndexes);

    return NO_ERRORS;
}

sortStatus merge(int *array, const size_t left, const size_t mid, const size_t right) {
    size_t leftSize  = mid   - left + 1;
    size_t rightSize = right - mid;

    int *leftSubarray = (int *)calloc(leftSize, sizeof(int));
    customWarning(leftSubarray, ALLOCATION_ERROR);

    int *rightSubarray = (int *)calloc(rightSize, sizeof(int));
    if (!rightSubarray) {
        FREE_(leftSubarray);
        customWarning(rightSubarray, ALLOCATION_ERROR);
    }

    for (size_t i = 0; i < leftSize; i++) {
        leftSubarray[i] = array[left + i];
    }

    for (size_t i = 0; i < rightSize; i++) {
        rightSubarray[i] = array[mid + 1 + i];
    }

    size_t i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (leftSubarray[i] <= rightSubarray[j]) {
            array[k++] = leftSubarray[i++];
        } else {
            array[k++] = rightSubarray[j++];
        }
    }

    while (i < leftSize) {
        array[k++] = leftSubarray[i++];
    }

    while (j < rightSize) {
        array[k++] = rightSubarray[j++];
    }

    FREE_(leftSubarray);
    FREE_(rightSubarray);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //q