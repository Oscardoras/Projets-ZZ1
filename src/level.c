#include "level.h"
#include <stdlib.h>

Level* new_level(int width, int height, int seed) {
    Level* level = malloc(sizeof(Level));
    if (level != NULL) {
        level->blocks = malloc(sizeof(Block) * width*height);
        if (level->blocks != NULL) {
            level->width = width;
            level->height = height;
            level->seed = seed;
            level->entities = NULL;
        } else free(level);
    }
    
    return level;
}

void free_level(Level* level) {
    if (level != NULL) {
        if (level->blocks != NULL)
            free(level->blocks);
        
        struct ListCell* cell = level->entities;
        while (cell != NULL) {
            struct ListCell* tmp = cell;
            cell = cell->next;
            free(tmp);
        }
        
        free(level);
    }
}

void resize_level(Level* level, int width, int height) {
    
}

Block* get_level_block(Level* level, int x, int y) {
    return &level->blocks[y*level->width + x];
}

bool add_level_entity(Level* level, Entity* entity) {
    struct ListCell* cell = malloc(sizeof(struct ListCell));
    if (cell != NULL) {
        cell->entity = entity;
        cell->next = level->entities;
        return true;
    } else return false;
}