#include <stdbool.h>
#include <stdlib.h>

#include "markov.h"


Matrix* load_matrix(FILE* file) {
    Matrix* matrix = malloc(sizeof(Matrix));

    if (matrix != NULL) { //If the allocationhas succeed.
        char buffer[1024];
        fgets(buffer, 1024, file);

        float values[512]; //The values on the first line of the matrix.
        matrix->size = 0;

        char value[1024];
        char* c = buffer;
        char* v = value;
        while (*c != '\0') {
            if (*c == ' ' || *c == '\n') {
                *v = '\0';
                values[matrix->size] = atof(value);
                matrix->size++;

                v = value;
            } else {
                *v = *c;
                v++;
            }
            c++;
        }
        //Now the size of the matrix is known.

        matrix->data = malloc(sizeof(float) * matrix->size*matrix->size);
        for (unsigned int j = 0; j < matrix->size; j++)
            matrix->data[j] = values[j];
        
        for (unsigned int i = 1; i < matrix->size; i++) { //Read the following lines.
            if (fgets(buffer, 1024, file)) {
                unsigned int j = 0;
                c = buffer+4;
                v = value;
                while (*c != '\0') {
                    if (*c == ' ' || *c == '\n') {
                        *v = '\0';
                        matrix->data[i*matrix->size + j] = atof(value);

                        v = value;
                        j++;
                    } else {
                        *v = *c;
                        v++;
                    }
                    c++;
                }
            } else {
                free(matrix->data);
                free(matrix);
                return NULL;
            }
        }
    }

    return matrix;
}

void free_matrix(Matrix* matrix) {
    if (matrix != NULL) {
        if (matrix->data != NULL) {
            free(matrix->data);
            matrix->data = NULL;
        }
        
        matrix->size = 0;
        free(matrix);
    }
}

float* get_matrix_element(Matrix* matrix, unsigned int i, unsigned int j) {
    return &matrix->data[i*matrix->size + j];
}

void forward_state(Matrix* matrix, State* state) {
    float random = rand() / (float) RAND_MAX;

    float sum = 0.;
    unsigned int j;
    for (j = 0; j < matrix->size; j++) {
        sum += *get_matrix_element(matrix, *state, j);
        if (random <= sum) {
            *state = j;
            return;
        }
    }
    
    *state = j-1;
}
