#ifndef SORT_CONTEXT_H_
#define SORT_CONTEXT_H_

#include <stdlib.h>
#include <pthread.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    if (ptr) {          \
        free(ptr);      \
        ptr = NULL;     \
    }                   \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

const int MOD = 1000000007;

const unsigned int SEED                  = 42;
const size_t       DEFAULT_THREADS_COUNT = 1;

const size_t DEFAULT_ARRAY_SIZE = 1;

enum sortStatus {
    NO_ERRORS                 = 0,
    ALLOCATION_ERROR          = 1,
    ARRAY_BAD_POINTER         = 2,
    THREADS_ARRAY_BAD_POINTER = 3,
    CONTEXT_BAD_POINTER       = 4,
    THREAD_CREATION_ERROR     = 5,
    CHUNK_BAD_POINTER         = 6,
    MERGE_ERROR               = 7,
    THREADS_BAD_POINTER       = 8,
    CHUNKS_BAD_POINTER        = 9,
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

struct Array {
    int   *array = {};
    size_t size  = {};
};

struct Chunk {
    int *array = {};

    size_t startIndex = 0;
    size_t endIndex   = 0;
};

struct PThreads {
    pthread_t *threads      = {};
    size_t     threadsCount = {};

    Chunk **chunks = {};
};

struct sortContext {
    Array    *data    = {};
    PThreads *threads = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus   initializeArray      (Array *array, const size_t size);
sortStatus   destroyArray         (Array *array);
sortStatus   generateRandomArray  (Array *array, const size_t size, const unsigned int seed);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

sortStatus   initializePThreads   (PThreads *threads, const size_t threadsCount);
sortStatus   destroyPThreads      (PThreads *threads);

sortContext *initializeSortContext(const size_t arraySize    = DEFAULT_ARRAY_SIZE, 
                                   const unsigned int seed   = SEED, 
                                   const size_t threadsCount = DEFAULT_THREADS_COUNT);
sortStatus   destroySortContext   (sortContext *context);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // SORT_CONTEXT_H_