#include <stdlib.h>

#include "entity.h"
#include "level.h"
#include "matrix.h"
#include "viewport.h"

#define WIDTH 800
#define HEIGHT 600
#define WORLD_X_MIN -80
#define WORLD_Y_MIN -60
#define WORLD_X_MAX 80
#define WORLD_Y_MAX 60
#define SEED 0
#define TEST_CREATION_FOURMI


int main(/*int argc, char** argv*/) {
    FILE* file = fopen("data.txt", "r");
    if (file) {
        load_types(file);
        fclose(file);
    }
    
    Position pos;
    pos.x = pos.y = pos.direction = 0;
    Entity* entity = new_entity(search_type("Ouvriere"), 0, 0, pos);
    
    Entity* entity_loaded = NULL;
    
    file = fopen("save.txt", "w+");
    if (file) {
        save_entity(entity, file);
        fseek(file, 0, 0);
        entity_loaded = load_entity(file);
    }

    Level* level = new_level(WORLD_X_MIN, WORLD_X_MAX, WORLD_Y_MIN, WORLD_Y_MAX, SEED);
    #ifdef TEST_CREATION_FOURMI
    {
        for(unsigned it = 0; it < 1000; ++it)
        {
            Position pos;
            pos.x = rand()%800;
            pos.y = rand()%600;
            pos.direction = rand()%360;
            add_level_entity(level, new_entity(&entity_types[0], 0, 100, pos));
        }
        
    }
    #endif
    Viewport* viewport = init_viewport(WIDTH, HEIGHT, level);
    event_loop(viewport);

    close_viewport(viewport);
    free_level(level);
    return EXIT_SUCCESS;
}