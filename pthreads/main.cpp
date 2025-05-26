#include <immintrin.h>
#include <stdlib.h>

#include "sortContext.h"
#include "sort.h"
#include "customWarning.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        customPrint(red, bold, bgDefault, "Usage: %s <array size> <threads count> [<seed> = 42]\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t arraySize    = strtoul(argv[1], NULL, 10);
    size_t threadsCount = strtoul(argv[2], NULL, 10);
    unsigned int seed   = (argc > 3) ? strtoul(argv[3], NULL, 10) : 42;

    sortContext *context = initializeSortContext(arraySize, seed, threadsCount);
    customWarning(context, EXIT_FAILURE);

    unsigned long long startTime = __rdtsc();
    sortArray(context);
    unsigned long long endTime   = __rdtsc();

    customPrint(green, bold, bgDefault, "Sorted %lu elements. Time: %llu, Threads count: %lu\n", endTime - startTime, endTime - startTime, threadsCount);

    destroySortContext(context);

    return EXIT_SUCCESS;
}