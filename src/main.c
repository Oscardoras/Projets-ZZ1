#include <stdlib.h>

#include "gameplay.h"
#include "entity.h"
#include "level.h"
#include "matrix.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600
#define WORLD_X_MIN -5
#define WORLD_Y_MIN -5
#define WORLD_X_MAX 5
#define WORLD_Y_MAX 5
#define SEED 0
#define TEST_CREATION_FOURMI 1


int main(/*int argc, char** argv*/) {
    FILE* file = fopen("data.txt", "r");
    if (file) {
        load_types(file);
        fclose(file);
    }



    Level* level = new_level(WORLD_X_MIN, WORLD_X_MAX, WORLD_Y_MIN, WORLD_Y_MAX, SEED);
    add_target(level, 2, 3, DIG);
    
    if(TEST_CREATION_FOURMI)
        for(unsigned int it = 0; it < 5; ++it) {
            Position pos;
            pos.x = rand()%10;
            pos.y = rand()%10;
            pos.direction = rand()%360;
            add_level_entity(level, new_entity(entity_types[0], 0, 100, pos));
        }
    
    
    
    Viewport* viewport = init_viewport(WIDTH, HEIGHT, level);
    event_loop(viewport);
    close_viewport(viewport);
    free_level(level);
    return EXIT_SUCCESS;
}