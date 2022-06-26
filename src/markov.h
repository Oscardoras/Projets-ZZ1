#ifndef __MARKOV_H__
#define __MARKOV_H__

#include <stdio.h>


/**
 * @brief Represents a Markov state.
 * 
 */
typedef unsigned int State;

/**
 * @brief Represents a Markov matrix.
 * 
 */
typedef struct {
    float *data;
    unsigned int size;
} Matrix;


/**
 * @brief Loads a matrix from a configuration file.
 * 
 * @param file the file containing the matrix.
 * @return the loaded matrix.
 */
Matrix* load_matrix(FILE* file);

/**
 * @brief Frees a matrix.
 * 
 * @param matrix the matrix to free.
 */
void free_matrix(Matrix* matrix);

/**
 * @brief Gets a pointer to a matrix element.
 * 
 * @param matrix a pointer to the matrix.
 * @param i the i coordinates.
 * @param j the j coordinates.
 * @return a pointer to the matrix element.
 */
float* get_matrix_element(Matrix* matrix, unsigned int i, unsigned int j);

/**
 * @brief Computes the new state.
 * 
 * @param matrix a pointer to the matrix.
 * @param state a pointer to the state.
 */
void forward_state(Matrix* matrix, State* state);


#endif
