#include <stdlib.h>

#include "customWarning.h"
#include "sortContext.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus generateRandomArray(Array *array, const size_t size, const unsigned int seed) {
    customWarning(array, ARRAY_BAD_POINTER);

    srand(seed);

    for (size_t i = 0; i < size; i++) {
        array->array[i] = rand() % MOD;
    }

    return NO_ERRORS;
}

sortStatus initializeArray(Array *array, const size_t size) {
    customWarning(array, ARRAY_BAD_POINTER);

    array->array = (int *)calloc(size, sizeof(int));
    customWarning(array->array, ALLOCATION_ERROR);

    array->size = size;

    return NO_ERRORS;
}

sortStatus destroyArray(Array *array) {
    customWarning(array, ARRAY_BAD_POINTER);

    FREE_(array->array);
    array->size = 0;

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus initializePThreads(PThreads *threads, const size_t threadsCount) {
    customWarning(threads, THREADS_ARRAY_BAD_POINTER);

    threads->threads = (pthread_t *)calloc(threadsCount, sizeof(pthread_t));
    customWarning(threads->threads, ALLOCATION_ERROR);

    threads->threadsCount = threadsCount;

    return NO_ERRORS;
}

sortStatus destroyPThreads(PThreads *threads) {
    customWarning(threads, THREADS_ARRAY_BAD_POINTER);

    FREE_(threads->threads);
    threads->threadsCount = 0;

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortContext *initializeSortContext(const size_t arraySize, const unsigned int seed, const size_t threadsCount) {
    sortContext *context = (sortContext *)calloc(1, sizeof(sortContext));
    customWarning(context, NULL);

    context->data = (Array *)calloc(1, sizeof(Array));

    if (!context->data) {
        FREE_(context);
        customWarning(context->data, NULL);
    }

    initializeArray(context->data, arraySize);

    generateRandomArray(context->data, arraySize, seed);

    context->threads = (PThreads *)calloc(1, sizeof(PThreads));

    if (!context->threads) {
        destroyArray(context->data);
        FREE_(context);

        customWarning(context->threads, NULL);
    }

    initializePThreads(context->threads, threadsCount);

    return context;
}

sortStatus destroySortContext(sortContext *context) {
    customWarning(context, CONTEXT_BAD_POINTER);

    if (context->data) {
        destroyArray(context->data);
    }

    if (context->threads) {
        destroyPThreads(context->threads);
    }

    FREE_(context);

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //