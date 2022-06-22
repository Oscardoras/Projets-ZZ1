#include "level.h"


Level* new_level(int width, int height, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        //level->entities = NULL;
        level->blocks = malloc(sizeof(Block) * width*height);
        level->width = width;
        level->height = height;
        level->seed = seed;
    }
    
    return level;
}