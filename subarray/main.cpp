#include <stdio.h>
#include <stdlib.h>

#include "customWarning.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static void printAnswer(long long maxSum, size_t maxLeft, size_t maxRight, int *array) {
    printf("Max sum: %lld\n", maxSum);
    printf("Subarray: [ ");

    for (size_t idx = maxLeft; idx <= maxRight; idx++) {
        printf("%d ", array[idx]);
    }

    printf("]\n");
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

int main() {
    size_t count;
    scanf("%lu", &count);

    if (count == 0) {
        printf("Max sum: %lld\n", 0LL);
        printf("Subarray: [ ]\n");
        return EXIT_SUCCESS;
    }

    long long maxSum     = 0;
    long long currentSum = 0;

    int *array = (int *)calloc(count, sizeof(int));
    customWarning(array, maxSum);

    for (size_t idx = 0; idx < count; idx++) {
        scanf("%d", &array[idx]);
    }

    maxSum = currentSum = array[0];

    size_t currLeft = {}, currRight = {};
    size_t maxLeft  = {}, maxRight  = {};

    for (size_t idx = 1; idx < count; idx++) {
        if (array[idx] > array[idx - 1]) {
            long long newSum   = currentSum + array[idx];
            long long newStart = array[idx];

            if (newSum > newStart) {
                currentSum = newSum;
                currRight  = idx;
            } else {
                currentSum = newStart;
                currLeft = currRight = idx;
            }
        } else {
            currentSum = array[idx];
            currLeft   = currRight = idx;
        }

        if (currentSum > maxSum) {
            maxSum   = currentSum;

            maxLeft  = currLeft;
            maxRight = currRight;
        }
    }

    printAnswer(maxSum, maxLeft, maxRight, array);

    FREE_(array);

    return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //