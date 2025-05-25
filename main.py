import numpy as np
import os

def generateMatrix(count, dimension=1000, minValue=-100, maxValue=100, outputDir="matrices"):
    if not os.path.exists(outputDir):
        os.makedirs(outputDir)

    for i in range(count):
        matrix = np.random.randint(minValue, maxValue + 1, size=(dimension, dimension), dtype=np.int32)
        
        filename = os.path.join(outputDir, f"matrix_{i+1}.txt")
        
        with open(filename, 'w') as f:
            flat_matrix = matrix.flatten()
            np.savetxt(f, flat_matrix, fmt='%d', delimiter=' ', newline=' ')

count = 2
matrixDimension = 1000

minValueUE = -100
maxValueUE =  100
outputDir  = "matrices"

generateMatrix(count, matrixDimension, minValueUE, maxValueUE, outputDir)