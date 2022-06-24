#include <stdbool.h>
#include <stdlib.h>

#include "matrix.h"

#define READ_BUFFER_SIZE 1000


Matrix load_matrix(FILE* file) {
    Matrix matrix;
    matrix.size = 0;
    char buffer[READ_BUFFER_SIZE];
    fgets(buffer, 1000, file); // compter le nombre de colonnes
    char * cour = buffer;
    bool prec_blank = ((*cour >= '0' && *cour <='9') || *cour == '.'  ? false : true );
    while(*cour != '\0')
    {
        bool blank = ( (*cour >= '0' && *cour <='9') || *cour == '.'  ? false : true );
        if(prec_blank && !blank)
        {
            ++matrix.size;
        }
        ++cour;
        prec_blank = blank;
    }
    matrix.data = (float*)malloc((sizeof(float)*matrix.size*matrix.size));
    if(!matrix.data)
    {
        printf("Erreur allocation\n");
        exit(EXIT_FAILURE);
    }
    float * floats = parse(buffer, matrix.size);
    for(unsigned int j = 0; j < matrix.size; ++j)
        *get_matrix_element(&matrix, 0, j) = floats[j];
    free(floats);
    for(unsigned int i = 1; i < matrix.size; ++i)
    {
        fgets(buffer, 1000, file);
        float * floats = parse(buffer, matrix.size);
        for(unsigned int j = 0; j < matrix.size; ++j)
            *get_matrix_element(&matrix, i, j) = floats[j];
        free(floats);
    }
    return matrix;
}

void free_matrix(Matrix* matrix) {
    free(matrix->data);
}

float* get_matrix_element(Matrix* matrix, unsigned int i, unsigned int j) {   
    return &matrix->data[i*matrix->size + j];
}

void forward_state(Matrix* matrix, State* state) {
    float random = (rand() % 1000) / 1000.;

    float sum = 0.;
    for (unsigned int j = 0; j <= matrix->size; j++) {
        sum += *get_matrix_element(matrix, *state, j);
        if (random <= sum) {
            *state = j;
            return;
        }
    }
    
    *state = matrix->size-1;
}

float* parse(char *string, unsigned int count) {
    float * floats = (float*)malloc(sizeof(float) * count);
    if(!floats)
    {
        printf("Erreur allocation\n");
        exit(EXIT_FAILURE);
    }
    char* cour = string;
    bool prec_blank = ((*cour >= '0' && *cour <='9') || *cour == '.'  ? false : true );
    char* begin = NULL;
    char* end = NULL;
    unsigned int current = 0;
    while(*cour != '\0')
    {
        bool blank = ( (*cour >= '0' && *cour <='9') || *cour == '.'  ? false : true );
        if(prec_blank && !blank)
        {
            //begin
            begin = cour;
        }
        else if(blank && !prec_blank)
        {
            end = cour;
            *end = '\0';
            floats[current] = atof(begin);
            ++current;
            //end
        }
        ++cour;
        prec_blank = blank;
    }
    return floats;
}