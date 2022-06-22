#include <stdlib.h>

#include "level.h"
#include "viewport.h"
#include "character.h"
#include "matrix.h"

#define WIDTH 800
#define HEIGHT 600
#define WORLD_X_MIN -80
#define WORLD_Y_MIN -60
#define WORLD_X_MAX 80
#define WORLD_Y_MAX 60
#define SEED 0

extern EntityType* entity_types[10];

int main(/*int argc, char** argv*/) {
    
    FILE* file = fopen("data.txt", "r");
    if(file) load_types(file);
    fclose(file);

    Level* level = new_level(WORLD_X_MIN, WORLD_X_MAX, WORLD_Y_MIN, WORLD_Y_MAX, SEED);
    Viewport* viewport = init_viewport(WIDTH, HEIGHT, level);
    event_loop(viewport);
    close_viewport(viewport);
    free_level(level);
    return EXIT_SUCCESS;
}