#ifndef __MARKOV_H__
#define __MARKOV_H__

#include <stdio.h>


typedef unsigned int State;

typedef struct {
    float *data;
    unsigned int size;
} matrix_t;

/**
 * @brief Creates a new matrix
 * 
 * @param file the file containing the matrix.
 * @return the loaded matrix.
 */
matrix_t init(FILE* file);

/**
 * @brief Frees a matrix.
 * 
 * @param matrix La matrice à libérer
 */
void close(matrix_t* matrix);

/**
 * @brief Get a pointer to a matrix element.
 * 
 * @param matrix a pointer to the matrix.
 * @param i the i coordinates.
 * @param j the j coordinates.
 * @return a pointer to the matrix element.
 */
float* get(matrix_t* matrix, unsigned int i, unsigned int j);

/**
 * @brief Alloue dynamiquement de l'espace et copie les données dans une nouvelle matrice
 * 
 * @param matrix La matrice dont il faut copier les donnéees
 * @return La nouvelle matrice
 */
matrix_t copy(matrix_t* matrix);

/**
 * @brief Computes the new state.
 * 
 * @param matrix a pointer to the matrix.
 * @param state a pointer to the state.
 */
void forward(matrix_t *markov, unsigned int *currentState);

/**
 * @brief Parse une chaîne de caractères afin de récupérer tous les flotants
 * 
 * @param string la chaîne de caractères
 * @return Un tableau de flotants
 */
float* parse(char* string, unsigned int count);
#endif
