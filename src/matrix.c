#include <SDL2/SDL.h>
#include <stdbool.h>

#include "matrix.h"

#define READ_BUFFER_SIZE 1000


matrix_t initMatrix(FILE *file) {
    matrix_t matrix;
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
        *getMatrix(&matrix, 0, j) = floats[j];
    free(floats);
    for(unsigned int i = 1; i < matrix.size; ++i)
    {
        fgets(buffer, 1000, file);
        float * floats = parse(buffer, matrix.size);
        for(unsigned int j = 0; j < matrix.size; ++j)
            *getMatrix(&matrix, i, j) = floats[j];
        free(floats);
    }
    return matrix;
}

void closeMatrix(matrix_t* matrix) {
    free(matrix->data);
}

float* getMatrix(matrix_t* matrix, unsigned int i, unsigned int j) {   
    return &matrix->data[i*matrix->size + j];
}

matrix_t copyMatrix(matrix_t* matrix)
{
    matrix_t newMatrix;
    newMatrix.size = matrix->size;
    newMatrix.data = (float*)malloc(sizeof(float)*newMatrix.size*newMatrix.size);
    if(newMatrix.data)
    {
        for(unsigned int it = 0; it < newMatrix.size*newMatrix.size; ++it)
        {
            newMatrix.data[it] = matrix->data[it];
        }
    }
    else{
        printf("Erreur alloc dynamique\n");
        exit(EXIT_FAILURE);
    }
    return newMatrix;
}

void forward(matrix_t* matrix, State* currentState) {
    if(*currentState >= markov->size)
    {
        printf("Erreur etat non existant\n");
        exit(EXIT_FAILURE);
    }
    float *Densites = (float*)malloc(sizeof(float)*markov->size);
    if(!Densites)
    {
        printf("Erreur allocation\n");
        exit(EXIT_FAILURE);
    }
    float random = (float)(rand()%1000)/1000.0;
    for(int it = markov->size-1; it >= 0; --it)
    {
        Densites[it] = *getMatrix(markov, *currentState, it);
        for(unsigned int it2 = 0; it2 < (unsigned int)it; ++it2)
        {
            Densites[it] += Densites[it2];
        }
    }
    int found = 0;
    for(unsigned int it = 0; it < markov->size && !found; ++it)
    {
        if(random < Densites[it]){
            *currentState = it;
            found = 1;
        }
    }
    free(Densites);
}

float* parse(char* string, unsigned int count) {
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