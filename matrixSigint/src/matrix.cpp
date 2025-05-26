#include <fcntl.h>
#include <signal.h>

#include "customWarning.h"
#include "matrix.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define FREE_(ptr) do { \
    free(ptr);          \
    ptr = NULL;         \
} while (0)

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

volatile sig_atomic_t currentI = 0;
volatile sig_atomic_t currentJ = 0;

volatile sig_atomic_t sigintReceived = 0;

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Matrix *initializeMatrix(const size_t dimension) {
    Matrix *matrix = (Matrix *)calloc(1, sizeof(Matrix));
    customWarning(matrix, NULL);

    matrix->dimension = dimension;

    matrix->array = (int *)calloc(dimension * dimension, sizeof(int));
    customWarning(matrix->array, NULL);

    return matrix;
}

matrixError destroyMatrix(Matrix *matrix) {
    customWarning(matrix, MATRIX_BAD_POINTER);

    if (matrix->array) {
        FREE_(matrix->array);
    }

    matrix->dimension = {};

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

matrixError readMatrixFromFile(Matrix *matrix, const char *path) {
    customWarning(matrix, MATRIX_BAD_POINTER);
    customWarning(path,   PATH_BAD_POINTER);

    int fileDescriptor = open(path, O_RDONLY);
    customWarning(fileDescriptor != -1, READ_MATRIX_ERROR);

    read(fileDescriptor, matrix->array, matrix->dimension * matrix->dimension * sizeof(int));

    close(fileDescriptor);

    return NO_ERRORS;
}

matrixError fillMatrix(Matrix *matrix, const sourceData *parameter) {
    customWarning(matrix,    MATRIX_BAD_POINTER);
    customWarning(parameter, PARAMETER_BAD_POINTER);

    if (matrix->dimension == 0) {
        return NO_ERRORS;
    }

    if (parameter->type == PATH) {
        matrixError status = readMatrixFromFile(matrix, parameter->data.path);
        customWarning(status == NO_ERRORS, READ_MATRIX_ERROR);
    } else {
        customWarning(parameter->data.fileDescriptor >= 0, READ_MATRIX_ERROR);

        for (size_t i = 0; i < matrix->dimension * matrix->dimension; i++) {
            scanf("%d", &matrix->array[i]);
        }
    }

    return NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Matrix *multiplyMatrices(const Matrix *matrixA, const Matrix *matrixB) {
    customWarning(matrixA, NULL);
    customWarning(matrixB, NULL);

    if (matrixA->dimension == 0 || matrixB->dimension == 0) {
        return NULL;
    }

    if (matrixA->dimension != matrixB->dimension) {
        return NULL;
    }

    size_t dimension = matrixA->dimension;

    Matrix *matrixC = initializeMatrix(dimension);
    customWarning(matrixC, NULL);

    matrixC->dimension = dimension;

    #if defined(__x86_64__) || defined(__amd64__) || defined(__AVX2__)

    int *a = matrixA->array;
    int *b = matrixB->array;
    int *c = matrixC->array;

    for (size_t i = 0; i < dimension; i++) {
        currentI = i;

        int *cRow = c + i * dimension;

        for (size_t j = 0; j < dimension; j++) {
            cRow[j] = 0;
        }

        for (size_t k = 0; k < dimension; k++) {
            int *bRow = b + k * dimension;

            __m256i aVec = _mm256_set1_epi32(a[i * dimension + k]);

            size_t j = 0;

            for (; j + 16 <= dimension; j += 16) {
                currentJ = j;

                __m256i bVec = _mm256_loadu_si256((__m256i *)(bRow + j));
                __m256i cVec = _mm256_loadu_si256((__m256i *)(cRow + j));

                __m256i tempVec = _mm256_mullo_epi32(aVec, bVec);
                tempVec = _mm256_add_epi32(cVec, tempVec);

                _mm256_storeu_si256((__m256i *)(cRow + j), tempVec);

                bVec = _mm256_loadu_si256((__m256i *)(bRow + j + 8));
                cVec = _mm256_loadu_si256((__m256i *)(cRow + j + 8));

                tempVec = _mm256_mullo_epi32(aVec, bVec);
                tempVec = _mm256_add_epi32(cVec, tempVec);

                _mm256_storeu_si256((__m256i *)(cRow + j + 8), tempVec);
            }

            for (; j < dimension; j++) {
                currentJ = j;

                cRow[j] += a[i * dimension + k] * bRow[j];
            }
        }
    }

    #else

    for (size_t i = 0; i < dimension; i++) {
        currentI = i;

        for (size_t j = 0; j < dimension; j++) {
            currentJ = j;

            matrixC->array[i * dimension + j] = 0;

            for (size_t k = 0; k < dimension; k++) {
                matrixC->array[i * dimension + j] += matrixA->array[i * dimension + k] * matrixB->array[k * dimension + j];
            }
        }
    }

    #endif

    return matrixC;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //