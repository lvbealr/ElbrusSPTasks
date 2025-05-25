#include "include/matrix.h"
#include "matrix.h"
#include "interruption.h"

static const size_t MATRIX_SIZE = 1000;

int main(int argc, char *argv[]) {
    setSignalHandler();
    // setSigactionHandler();

    Matrix *matrixA = initializeMatrix(MATRIX_SIZE);
    Matrix *matrixB = initializeMatrix(MATRIX_SIZE);

    // sourceData sourceA = {FILE_DESCRIPTOR, {.fileDescriptor = STDIN_FILENO}};
    // sourceData sourceB = {FILE_DESCRIPTOR, {.fileDescriptor = STDIN_FILENO}};

    sourceData sourceA = {PATH, {.path = "matrices/matrix_1.txt"}};
    sourceData sourceB = {PATH, {.path = "matrices/matrix_2.txt"}};

    fillMatrix(matrixA, &sourceA);
    fillMatrix(matrixB, &sourceB);

    Matrix *matrixC = multiplyMatrices(matrixA, matrixB);

    destroyMatrix(matrixA);
    destroyMatrix(matrixB);
    destroyMatrix(matrixC);
}