#ifndef MATRIX_H_
#define MATRIX_H_

#include <unistd.h>
#include <stdlib.h>
#include <immintrin.h>

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum matrixError {
    NO_ERRORS                 = 0,
    ALLOCATION_ERROR          = 1,
    MATRIX_BAD_POINTER        = 2,
    READ_MATRIX_ERROR         = 3,
    PATH_BAD_POINTER          = 4,
    PARAMETER_BAD_POINTER     = 5
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

struct Matrix {
    int   *array     = {};
    size_t dimension = {};
};

enum sourceType {
    PATH            = 0,
    FILE_DESCRIPTOR = 1
};

union Source {
    const char *path;
    int fileDescriptor = STDIN_FILENO;
};

struct sourceData {
    sourceType type = {};
    Source     data = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Matrix     *initializeMatrix(const size_t  dimension);
matrixError destroyMatrix   (Matrix *matrix);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

matrixError readMatrixFromFile(Matrix *matrix, const char *path);
matrixError fillMatrix        (Matrix *matrix, const sourceData *parameter);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

Matrix *multiplyMatrices(const Matrix *matrixA, const Matrix *matrixB);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // MATRIX_H_