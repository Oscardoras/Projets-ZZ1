#include <stdlib.h>

#include "level.h"
#include "viewport.h"
#include "character.h"
#include "matrix.h"

extern EntityType** entity_types;

int main(/*int argc, char** argv*/) {
    FILE* file = fopen("data.txt", "r");
    if(file) load_types(file);
    fclose(file);
    return EXIT_SUCCESS;
}