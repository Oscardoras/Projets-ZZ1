#ifndef __MARKOV_H__
#define __MARKOV_H__
#include <stdio.h>
typedef struct {
    float* data;
    unsigned int size;
} matrix_t;

/**
 * @brief create a new matrix
 * 
 * @param fileName Le nom du fichier
 * @return La matrice créée
 */
matrix_t init(FILE * file);

/**
 * @brief Libère la mémoire en détruisant une matrice
 * 
 * @param matrix La matrice à libérer
 */
void close(matrix_t& matrix);

/**
 * @brief Récupère une case d'une matrice
 * 
 * @param matrix La matrice dont il fautr récupérer la donnée
 * @param i coordonnée i
 * @param j coordonnée j
 * @return Un pointeur sur la donnée que l'on récupère
 */
float* get(matrix_t& matrix, unsigned int i, unsigned int j);

/**
 * @brief Alloue dynamiquement de l'espace et copie les données dans une nouvelle matrice
 * 
 * @param matrix La matrice dont il faut copier les donnéees
 * @return La nouvelle matrice
 */
matrix_t copy(matrix_t& matrix);

/**
 * @brief Calcule un nouvel état markovien à partir d'une matrice de probabilités
 * 
 * @param markov La matrice de probabilités markovienne
 * @param currentState L'état courant
 */
void forward(matrix_t& markov, unsigned int &currentState);
#endif
