#include <stdlib.h>

#include "gameplay.h"
#include "entity.h"
#include "level.h"
#include "markov.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600
#define WORLD_X_MIN -40
#define WORLD_Y_MIN -30
#define WORLD_X_MAX 40
#define WORLD_Y_MAX 30
#define SEED 0


int main(/*int argc, char** argv*/) {

    FILE* file = fopen("configuration.txt", "r");
    if (file) {
        load_types(file);
        fclose(file);
    } else {
        exit(EXIT_FAILURE);
    }


    Level* level = new_level(WORLD_X_MIN, WORLD_X_MAX, WORLD_Y_MIN, WORLD_Y_MAX, SEED);
    if (level == NULL) {
        exit(EXIT_FAILURE);
    }

    /*
    Position queen_position;
    queen_position.x = 0;
    queen_position.y = 1;
    queen_position.direction = LEFT;
    add_level_entity(level, new_entity(QUEEN, queen_position, 0, 1));


    //add_pheromone(level, 1, 2, PHEROMONE_DIG);
    */
    
    Location location;
    location.x = 3;
    location.y = 0;
    location.direction = DIRECTION_LEFT;
    add_level_entity(level, new_entity(ENTITY_WORKER, location, 1));

    
    Viewport* viewport = create_viewport(WIDTH, HEIGHT, level);
    if (viewport == NULL) {
        free_level(level);
        exit(EXIT_FAILURE);
    }
    
    event_loop(viewport);

    close_viewport(viewport);
    free_level(level);

    return EXIT_SUCCESS;
}