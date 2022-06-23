#include <stdlib.h>

#include "gameplay.h"
#include "entity.h"
#include "level.h"
#include "matrix.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600
#define WORLD_X_MIN -40
#define WORLD_Y_MIN -30
#define WORLD_X_MAX 50
#define WORLD_Y_MAX 50
#define SEED 0
#define TEST_CREATION_FOURMI 1


int main(/*int argc, char** argv*/) {
    FILE* file = fopen("data.txt", "r");
    if (file) {
        load_types(file);
        fclose(file);
    }



    Level* level = new_level(WORLD_X_MIN, WORLD_X_MAX, WORLD_Y_MIN, WORLD_Y_MAX, SEED);
    add_pheromone(level, 1, 2, PHEROMONE_DIG);
    
    if(TEST_CREATION_FOURMI)
        for(unsigned int it = 0; it < 1; ++it) {
            Position pos;
            pos.x = 3;//rand()%10;
            pos.y = 0;//rand()%10;
            pos.direction = rand()%360;
            add_level_entity(level, new_entity(WORKER, pos, 100, 1));
        }
    
    
    
    Viewport* viewport = init_viewport(WIDTH, HEIGHT, level);
    event_loop(viewport);
    close_viewport(viewport);
    free_level(level);
    return EXIT_SUCCESS;
}