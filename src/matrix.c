#include "matrix.h"
matrix_t init(FILE *file)
{
    matrix_t matrix;
}

void close(matrix_t *matrix)
{
    free(matrix.data);
}

float* get(matrix_t *matrix, unsigned int i, unsigned int j)
{   
    return &matrix.data[matrix.size*i+j];
}

matrix_t copy(matrix_t *matrix)
{
    matrix_t newMatrix;
    newMatrix.size = matrix.size;
    newMatrix.data = (float*)malloc(sizeof(float)*newMatrix.size*newMatrix.size);
    if(newMatrix.data)
    {
        for(unsigned int it = 0; it < newMatrix.size*newMatrix.size; ++it)
        {
            newMatrix.data[it] = matrix.data[it];
        }
    }
    else{
        printf("Erreur alloc dynamique\n");
        exit(EXIT_FAILURE);
    }
}

void forward(matrix_t *markov, unsigned int &currentState)
{

}